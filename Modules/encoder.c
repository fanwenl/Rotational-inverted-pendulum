#include "encoder.h"
#include "gpio.h"
#include "util.h"
#include "usart.h"
#include "timer.h"
#include "lqr.h"
#include "motor.h"

float x1 = 0;	//旋臂角度(度)
float x2 = 0;	//摆杆角度(度)
float x3 = 0;	//旋臂角速度(度/秒)
float x4 = 0;	//摆杆角速度(度/秒)

static uint32_t get_cnt_buffer_last_cnt(uint8_t index, uint32_t *p_cnt_buffer);
static float calc_angle_speed1(uint32_t cnt);
static float calc_angle_speed2(uint32_t cnt);

//电机编码器
void encoder1_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//TIM2_CH1->PA15, TIM2_CH2->PB3
	gpio_af_od_up_init(GPIOA, GPIO_Pin_15);	//编码器AB相输入口,复用开漏上拉
	gpio_af_od_up_init(GPIOB, GPIO_Pin_3);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2);
	
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_Period = ENCODER_RELOAD;	//TIM2为32位定时器,这里把重载值设为最大，可以不考虑计数溢出
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	TIM2->CNT = COUNTER1_RESET;
	TIM_Cmd(TIM2, ENABLE);
}

//摆杆编码器
void encoder2_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
	//TIM5_CH1->PA0, TIM5_CH2->PA1
	gpio_af_od_up_init(GPIOA, GPIO_Pin_0|GPIO_Pin_1);	//编码器AB相输入口,复用开漏上拉
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);
	
	TIM_DeInit(TIM5);
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_Period = ENCODER_RELOAD;	//TIM5为32位定时器,这里把重载值设为最大，可以不考虑计数溢出
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM5, &TIM_ICInitStructure);
	
	TIM5->CNT = COUNTER2_RESET;
	TIM_Cmd(TIM5, ENABLE);
}

#define BUFFER_HEAD1    0X55
#define BUFFER_HEAD2    0X66
#define BUFFER_SIZE     14
void send_x1_x2(void)
{
	uint32_t int_x1, int_x2, int_u;
	uint8_t buffer[BUFFER_SIZE];
	buffer[0] = BUFFER_HEAD1;
	buffer[1] = BUFFER_HEAD2;
	int_x1 = (x1+180)*1000+0.5f;
	int_x2 = (x2+180)*1000+0.5f;
	int_u = (u+1000)*1000+0.5f;
	buffer[2] = int_x1>>24;
	buffer[3] = int_x1>>16;
	buffer[4] = int_x1>>8;
	buffer[5] = int_x1;
	buffer[6] = int_x2>>24;
	buffer[7] = int_x2>>16;
	buffer[8] = int_x2>>8;
	buffer[9] = int_x2;
	buffer[10] = int_u>>24;
	buffer[11] = int_u>>16;
	buffer[12] = int_u>>8;
	buffer[13] = int_u;
	usart_sendBytes(UART4, buffer, BUFFER_SIZE);
}

extern uint8_t start_flag;
float pwm;
void TIM6_DAC_IRQHandler(void)
{
//	static uint32_t last1 = COUNTER_RESET;
//	static uint32_t last2 = COUNTER_RESET;
	
	uint32_t cnt1, cnt2;
	int32_t tmp1, tmp2;
	float tmp;
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
		cnt1 = TIM2->CNT;
		cnt2 = TIM5->CNT;
		tmp1 = (int32_t)(cnt1-COUNTER1_RESET)>=0? (cnt1-COUNTER1_RESET)%(4*ENCODER1_PPR) : 4*ENCODER1_PPR-(COUNTER1_RESET-cnt1)%(4*ENCODER1_PPR);
		tmp2 = (int32_t)(cnt2-COUNTER2_RESET)>=0? (cnt2-COUNTER2_RESET)%(4*ENCODER2_PPR) : 4*ENCODER2_PPR-(COUNTER2_RESET-cnt2)%(4*ENCODER2_PPR);
		tmp = tmp1/(4.0f*ENCODER1_PPR)*360.0f;
		x1 = (tmp>180)? 360-tmp : -tmp;
		x2 = -(tmp2/(4.0f*ENCODER2_PPR)*360.0f-180.0f);
		
		x3 = -calc_angle_speed1(cnt1);
		x4 = -calc_angle_speed2(cnt2);
		
//		x3 = (float)((int32_t)(last1-cnt1)/(4.0f*ENCODER1_PPR)*360.0f*100.0f);
//		last1 = cnt1;
//		x4 = (float)((int32_t)(last2-cnt2)/(4.0f*ENCODER2_PPR)*360.0f*100.0f);
//		last2 = cnt2;
		
		pwm = lqr_get_pwm();
		if(start_flag){
			motor_set_pwm(pwm);
		}
		else{
			motor_set_pwm(0);
		}
		send_x1_x2();
	}
}


#define CNT_BUFFER_SIZE	2
static uint32_t get_cnt_buffer_last_cnt(uint8_t index, uint32_t *p_cnt_buffer)
{
	uint32_t res;
	res = (index == CNT_BUFFER_SIZE-1)? 0 : index+1;
	return p_cnt_buffer[res];
}

static float calc_angle_speed1(uint32_t cnt)
{
	static uint32_t cnt_buffer[CNT_BUFFER_SIZE] = {0};
	static uint8_t index = 0;
	float angle_speed;
	cnt_buffer[index] = cnt;
	angle_speed = (float)((int32_t)(cnt-get_cnt_buffer_last_cnt(index, cnt_buffer))/(4.0f*ENCODER1_PPR)*360.0f*100/(CNT_BUFFER_SIZE));
	index = (index == CNT_BUFFER_SIZE-1)? 0 : index+1;
	return angle_speed;
}

static float calc_angle_speed2(uint32_t cnt)
{
	static uint32_t cnt_buffer[CNT_BUFFER_SIZE] = {0};
	static uint8_t index = 0;
	float angle_speed;
	cnt_buffer[index] = cnt;
	angle_speed = (float)((int32_t)(cnt-get_cnt_buffer_last_cnt(index, cnt_buffer))/(4.0f*ENCODER2_PPR)*360.0f*100/(CNT_BUFFER_SIZE));
	index = (index == CNT_BUFFER_SIZE-1)? 0 : index+1;
	return angle_speed;
}
