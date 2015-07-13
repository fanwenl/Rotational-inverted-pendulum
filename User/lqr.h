#ifndef __LQR_H__
#define __LQR_H__

//反馈增益矩阵
#define LQR_K1	(1.0f)
#define LQR_K2	(1.0f)
#define LQR_K3	(1.0f)
#define LQR_K4	(1.0f)

//电机输入电压最大值
#define U_MAX	(12)

float lqr_get_pwm(void);

#endif



