#include "encoder.h"
#include "gpio.h"
#include "util.h"
#include "usart.h"

void encoder_init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	//TIM2_CH1->PA5, TIM2_CH2->PA1
	gpio_af_od_up_init(GPIOA, GPIO_Pin_1|GPIO_Pin_5);	//编码器AB相输入口,复用开漏上拉
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_TIM2);
	
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
