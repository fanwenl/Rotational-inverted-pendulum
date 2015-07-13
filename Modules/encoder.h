#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "stm32f4xx_tim.h"

#define ENCODER_PPR 		600
#define ENCODER_RELOAD 		0XFFFFFFFF
#define COUNTER_RESET 		0X7FFFFFFF

extern float x1;	//旗글실똑(똑)
extern float x2;	//겠맷실똑(똑)
extern float x3;	//旗글실醵똑(똑/취)
extern float x4;	//겠맷실醵똑(똑/취)

void encoder1_init(void);
void encoder2_init(void);

#endif

