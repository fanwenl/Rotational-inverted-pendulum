#ifndef __LQR_H__
#define __LQR_H__

//反馈增益矩阵
extern float lqr_k1;
extern float lqr_k2;
extern float lqr_k3;
extern float lqr_k4;
extern float gain;

extern float u;

//电机输入电压最大值
#define U_MAX	(12)

float lqr_get_pwm(void);

#endif



