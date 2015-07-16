#ifndef __MICROS_TIME_16_H__
#define __MICROS_TIME_16_H__

#include "stm32f4xx_tim.h"

void micros_time_16_1_init(void);
uint32_t get_time1(void);
uint32_t get_interval_time1(void);

void micros_time_16_2_init(void);
uint32_t get_time2(void);
uint32_t get_interval_time2(void);

#endif

