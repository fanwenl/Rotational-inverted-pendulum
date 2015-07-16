#include "motor.h"
#include "pwm.h"

void motor_init(void)
{
	pwm_config(21000);
}

void motor_set_pwm(float pwm)
{
	float pwm1, pwm2;
	if(pwm>=0){
		pwm1 = pwm;
		pwm2 = 0;
	}
	else{
		pwm1 = 0;
		pwm2 = -pwm;
	}
	set_pwm(pwm1, pwm2);
}

