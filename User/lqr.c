#include "lqr.h"
#include "encoder.h"
#include "usart.h"

float u;
float lqr_k1 = -3.1623f;
float lqr_k2 = 26.7150f;
float lqr_k3 = -2.1897f;
float lqr_k4 = 2.7112f;
float gain = 60;

float lqr_get_pwm(void)
{
	u = (lqr_k1*x1+lqr_k2*x2+lqr_k3*x3+lqr_k4*x4)/gain;	//计算输出电压
	return u/U_MAX*100;								//计算电机PWM
}


