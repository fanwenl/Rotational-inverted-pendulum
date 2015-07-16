#include "stm32f4xx.h"
#include "util.h"
#include "usart.h"
#include "delay.h"
#include "encoder.h"
#include "nano_board.h"
#include "gpio.h"
#include "timer.h"
#include "motor.h"
#include "touch.h"
#include "gui.h"
#include "lcd.h"
#include "lqr.h"
#include "flash.h"

void read_parameter(void);

extern uint32_t overflew_vounter;
extern uint8_t start_flag;
extern float u;
uint8_t mask = 0;
int main()
{
	rcc_clock_enable();
	delay_config();
	uart4_init(115200);
	usart1_init(115200);
	//print_clock_freq();
	nano_board_led_init();
	
	motor_init();
	TIM6_init(100, 8400);		//10ms, ¼ÆËãÐý±ÛºÍ°Ú¸Ë½Ç¶È
	lcd_init();
	encoder1_init();
	encoder2_init();
	gui_drawHome();		

	read_parameter();
	//printf("gain=%6.3f, k1=%6.3f, k2=%6.3f, k3=%6.3f, k4=%6.3f\r\n", gain, lqr_k1, lqr_k2, lqr_k3, lqr_k4);
	
	while(1){
		//invoking();
		//printf("u=%10.3f, x1=%6.3f, x2=%6.3f, x3=%6.3f, x4=%6.3f\r\n", u, x1, x2, x3, x4);
		//printf("gain=%6.3f, k1=%6.3f, k2=%6.3f, k3=%6.3f, k4=%6.3f\r\n", gain, lqr_k1, lqr_k2, lqr_k3, lqr_k4);
		//delay_ms(250);
//		printf("%d\r\n", get_interval_time2());
		if(start_flag == 0 && mask == 0){
			read_parameter();
			mask = 1;
		}
		if(start_flag == 1){
			mask = 0;
		}
	}
}

void read_parameter(void)
{
	lqr_k1 = (flash_buffer32[0]/1000.0f)-1000.0f;
	lqr_k2 = (flash_buffer32[1]/1000.0f)-1000.0f;
	lqr_k3 = (flash_buffer32[2]/1000.0f)-1000.0f;
	lqr_k4 = (flash_buffer32[3]/1000.0f)-1000.0f;
	gain = (flash_buffer32[4]/1000.0f)-1000.0f;
}

