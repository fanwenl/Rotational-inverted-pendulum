#ifndef __LQR_H__
#define __LQR_H__

//�����������
#define LQR_K1	(1.0f)
#define LQR_K2	(1.0f)
#define LQR_K3	(1.0f)
#define LQR_K4	(1.0f)

//��������ѹ���ֵ
#define U_MAX	(12)

float lqr_get_pwm(void);

#endif



