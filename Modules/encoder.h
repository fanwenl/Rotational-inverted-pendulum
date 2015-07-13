#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "stm32f4xx_tim.h"

#define ENCODER_PPR 		600
#define ENCODER_RELOAD 		0XFFFFFFFF
#define COUNTER_RESET 		0X7FFFFFFF

extern float x1;	//���۽Ƕ�(��)
extern float x2;	//�ڸ˽Ƕ�(��)
extern float x3;	//���۽��ٶ�(��/��)
extern float x4;	//�ڸ˽��ٶ�(��/��)

void encoder1_init(void);
void encoder2_init(void);

#endif

