#include "stm32f4xx.h"
#include "util.h"
#include "usart.h"
#include "delay.h"
#include "encoder.h"
#include "stm32f4_discovery.h"
#include "gpio.h"
#include "timer.h"
#include "touch.h"
#include "spi.h"
#include "lcd.h"
#include "gui.h"

int main()
{
	rcc_clock_enable();
	delay_config();
	usart_config(UART4, 115200);
	print_clock_freq();
	//encoder_init();
	STM_EVAL_LEDInit(LED4);
	TIM7_init(10000, 8400);
	
	while(1){
		STM_EVAL_LEDToggle(LED4);
		delay_ms(1000);
		gui_drawHome();
	}
}

