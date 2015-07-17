#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "stm32f4xx_tim.h"

#define ENCODER1_PPR 		(500*16)		//电机编码器线数
#define ENCODER2_PPR 		(1000)			//摆杆编码器线数
#define ENCODER_RELOAD 		0XFFFFFFFF
#define COUNTER1_RESET 		(0X7FFFFFFF)
#define COUNTER2_RESET 		(0X7FFFFFFF)

extern float x1;	//旋臂角度(度)
extern float x2;	//摆杆角度(度)
extern float x3;	//旋臂角速度(度/秒)
extern float x4;	//摆杆角速度(度/秒)

void encoder1_init(void);
void encoder2_init(void);

#endif

