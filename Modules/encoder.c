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

void encoder1_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//TIM2_CH1->PA5, TIM2_CH2->PB3
	gpio_af_od_up_init(GPIOA, GPIO_Pin_5);	//编码器AB相输入口,复用开漏上拉
	gpio_af_od_up_init(GPIOB, GPIO_Pin_3);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_TIM2);
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
	
	TIM2->CNT = COUNTER_RESET;
	TIM_Cmd(TIM2, ENABLE);
}

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
	TIM_TimeBaseStructure.TIM_Period = ENCODER_RELOAD;	//TIM2为32位定时器,这里把重载值设为最大，可以不考虑计数溢出
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM5, &TIM_ICInitStructure);
	
	TIM5->CNT = COUNTER_RESET;
	TIM_Cmd(TIM5, ENABLE);
}

void TIM6_DAC_IRQHandler(void)
{
	static uint32_t last_counter1 = COUNTER_RESET;
	static uint32_t last_counter2 = COUNTER_RESET;
	uint32_t cnt1, cnt2;
	int32_t tmp1, tmp2;
	if(TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
		cnt1 = TIM2->CNT;
		cnt2 = TIM5->CNT;
		tmp1 = (int32_t)(cnt1-COUNTER_RESET)>=0? (cnt1-COUNTER_RESET)%(4*ENCODER_PPR) : 4*ENCODER_PPR-(COUNTER_RESET-cnt1)%(4*ENCODER_PPR);
		tmp2 = (int32_t)(cnt2-COUNTER_RESET)>=0? (cnt2-COUNTER_RESET)%(4*ENCODER_PPR) : 4*ENCODER_PPR-(COUNTER_RESET-cnt2)%(4*ENCODER_PPR);
		x1 = tmp1/(4.0f*ENCODER_PPR)*360.0f;
		x2 = tmp2/(4.0f*ENCODER_PPR)*360.0f;
		x3 = (float)((int32_t)(cnt1-last_counter1)/(4.0f*ENCODER_PPR)*360.0f*100.0f);
		x4 = (float)((int32_t)(cnt2-last_counter2)/(4.0f*ENCODER_PPR)*360.0f*100.0f);
		last_counter1 = cnt1;
		last_counter2 = cnt2;
		motor_set_pwm(lqr_get_pwm());
	}
}
