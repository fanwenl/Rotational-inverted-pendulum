#include "timer.h"
#include "util.h"
#include "usart.h"

//TIM9, TIM10, TIM11
void timer_configuration(TIM_TypeDef* timx, uint32_t period, uint16_t prescaler)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
	uint8_t irq_hannel;
	uint8_t priority = 5;
	
	if(timx == TIM9){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE);//APB2高速外设
		irq_hannel = TIM1_BRK_TIM9_IRQn;
		priority = 5;
	}
	else if(timx == TIM10){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);//APB2高速外设
		irq_hannel = TIM1_UP_TIM10_IRQn;
		priority = 6;
	}
	else if(timx == TIM11){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);//APB2高速外设
		irq_hannel = TIM1_TRG_COM_TIM11_IRQn;
		priority = 7;
	}
	else{
		while(1);
	}
  
    TIM_TimeBaseStructure.TIM_Period = period-1;    
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler-1;                
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;             
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(timx, &TIM_TimeBaseStructure);  
    TIM_Cmd (timx, ENABLE);
    TIM_ClearFlag(timx, TIM_FLAG_Update);
	nvic_configuration(irq_hannel, priority);
    TIM_ITConfig(timx, TIM_IT_Update, ENABLE);
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM9, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM9, TIM_IT_Update);
		printf("hello world! ");
	}
}

void TIM1_UP_TIM10_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM10, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM10, TIM_IT_Update);
		printf("hello \r\n");
	}
}

void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM11, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM11, TIM_IT_Update);
		printf("world \r\n");
	}
}

