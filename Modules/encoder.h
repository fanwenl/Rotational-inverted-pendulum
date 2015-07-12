#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "stm32f4xx_tim.h"

#define ENCODER_PPR 		600
#define ENCODER_RELOAD 		0XFFFFFFFF
#define COUNTER_RESET 		0X7FFFFFFF

void encoder_init(void);

#endif

