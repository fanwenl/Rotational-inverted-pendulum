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
#include "touch.h"
#include "gui.h"
#include "lcd.h"

int main()
{
	rcc_clock_enable();
	delay_config();
	uart4_init(115200);
	print_clock_freq();
	nano_board_led_init();
	
	motor_init();
	TIM6_init(100, 840);//1ms
	lcd_init();
	
	encoder1_init();
	encoder2_init();
	
	gui_drawHome();
	
	while(1){
		//invoking();
		printf("x1=%6.3f, x2=%6.3f, x3=%6.3f, x4=%6.3f\r\n", x1, x2, x3, x4);
		delay_ms(250);
	}
}

