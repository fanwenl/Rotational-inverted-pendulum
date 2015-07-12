#ifndef __PWM_H__
#define __PWM_H__

#include "stm32f4xx_tim.h"

void pwm_config(uint16_t freq);
void set_pwm1(u16 compare);
void set_pwm2(u16 compare);
void set_pwm3(u16 compare);
void set_pwm4(u16 compare);
void set_pwm(u16 val1, u16 val2, u16 val3, u16 val4);


#endif


