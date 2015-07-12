#ifndef __TIMER_H__
#define __TIMER_H__

#include "stm32f4xx.h"

void timer_configuration(TIM_TypeDef* timx, uint32_t period, uint16_t prescaler);

#endif
