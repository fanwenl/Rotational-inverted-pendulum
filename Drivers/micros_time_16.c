#include "micros_time_16.h"
#include "util.h"

uint32_t overflew_counter1 = 0;
uint32_t overflew_counter2 = 0;

void micros_time_16_1_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 0XFFFF;    
    TIM_TimeBaseStructure.TIM_Prescaler = (84-1);                
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;             
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);  
    TIM_Cmd(TIM14, ENABLE);
	TIM_ITConfig(TIM14, TIM_IT_Update, ENABLE);
	nvic_config(TIM8_TRG_COM_TIM14_IRQn, 5);
}

void TIM8_TRG_COM_TIM14_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM14, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM14, TIM_IT_Update);
		overflew_counter1++;
	}
}

uint32_t get_time1(void)
{
	return overflew_counter1*65536+TIM14->CNT;
}

uint32_t get_interval_time1(void)
{
	static uint32_t last_time = 0;
	uint32_t res, curr;
	curr = get_time1();
	res = curr-last_time;
	last_time = curr;
	return res;
}

void micros_time_16_2_init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
	
	TIM_TimeBaseStructure.TIM_Period = 0XFFFF;    
    TIM_TimeBaseStructure.TIM_Prescaler = (84-1);                
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;             
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM13, &TIM_TimeBaseStructure);  
    TIM_Cmd(TIM13, ENABLE);
	TIM_ITConfig(TIM13, TIM_IT_Update, ENABLE);
	nvic_config(TIM8_UP_TIM13_IRQn, 6);
}

void TIM8_UP_TIM13_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM13, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM13, TIM_IT_Update);
		overflew_counter2++;
	}
}

uint32_t get_time2(void)
{
	return overflew_counter2*65536+TIM13->CNT;
}

uint32_t get_interval_time2(void)
{
	static uint32_t last_time = 0;
	uint32_t res, curr;
	curr = get_time2();
	res = curr-last_time;
	last_time = curr;
	return res;
}

