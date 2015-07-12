#include "stm32f4xx.h"
#include "util.h"
#include "usart.h"
#include "delay.h"

int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	rcc_clock_enable();
	delay_configuration();
	usart_configuration(UART4, 115200);
	print_clock_freq();
	
	while(1){
		delay_ms(250);
	}
}

