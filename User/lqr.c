#include "lqr.h"
#include "encoder.h"

float lqr_get_pwm(void)
{
	float u;
	u = LQR_K1*x1+LQR_K2*x2+LQR_K3*x3+LQR_K4*x4;	//���������ѹ
	return u/U_MAX*100;								//������PWM
}


