#include "stm32f4xx.h"
#include "util.h"
#include "usart.h"
#include "delay.h"
#include "encoder.h"
#include "nano_board.h"
#include "gpio.h"
#include "timer.h"
#include "motor.h"
#include "micros_time_16.h"

int main()
{
//	float duty_cycle = 0;
	rcc_clock_enable();
	delay_config();
	uart4_init(115200);
	print_clock_freq();
	nano_board_led_init();
	
//	micros_time_16_init();
	encoder1_init();
	encoder2_init();
	motor_init();
	TIM6_init(100, 8400);//10ms
	
	while(1){
//		printf("input duty cycle: ");
//		scanf("%f", &duty_cycle);
//		printf("\r\n");
//		motor_set_pwm(duty_cycle);
	}
}

