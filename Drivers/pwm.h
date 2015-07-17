#ifndef __PWM_H__
#define __PWM_H__

#include "stm32f4xx_tim.h"
#include "bitband.h"

#define MOTOR_IN1	PeOutBit(1)
#define MOTOR_IN2	PeOutBit(2)

void pwm_config(uint32_t freq);
void set_pwm1(float compare);
void set_pwm2(float compare);
void set_pwm(float pwm1, float pwm2);

#endif


