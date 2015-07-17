#include "gui.h"
#include "lcd.h"
#include "usart.h"

void gui_drawIcon(uint16_t x, uint16_t y, char *name, uint16_t color)
{
	lcd_draw_rectangle(x-32, y-48, x+32, y+16, color);
	lcd_draw_circle(x, y-16, 24, color);
	lcd_draw_line(x, y-48, x, y+16, color);
	lcd_draw_line(x-32, y-16, x+32, y-16, color);
	lcd_show_str(x-32, y+16, name, GRAYBLUE);
}

void gui_drawHome(void)
{
	lcd_draw_line(0, 290, 239, 290, BLACK);
	lcd_show_str(88, 290, "HOME", BLUE);
	lcd_fill(APP1_X-20, APP1_Y, APP1_X+140, APP1_Y+65, GRAYBLUE);
	lcd_show_str(APP1_X, APP1_Y, "Swinging       around", BRRED);
	lcd_fill(APP2_X-20, APP2_Y, APP2_X+140, APP2_Y+65, GRAYBLUE);
	lcd_show_str(APP2_X, APP2_Y, "Starts        swinging", BRRED);
	lcd_fill(APP3_X-20, APP3_Y, APP3_X+140, APP3_Y+65, GRAYBLUE);
	lcd_show_str(APP3_X, APP3_Y, "Inverted       pendulum", BRRED);
	lcd_fill(APP4_X-20, APP4_Y, APP4_X+140, APP4_Y+65, GRAYBLUE);
	lcd_show_str(APP4_X, APP4_Y, "Circular       motion", BRRED);
}

uint8_t isInRange(uint16_t xPos, uint16_t yPos, uint16_t appx, uint16_t appy)
{
	if(xPos > 0&& xPos < 239 && yPos > appy && yPos < appy+65){
		return 1;
	}
	else{
		return 0;
	}
}

#include "motor.h"
#include "delay.h"
#include "encoder.h"
#include <math.h>
#define PWM_GAIN	(80)
#define PI			(3.1415926f)
void side_to_side(void)
{
	static int8_t dir_flag = 0;
	float Ep, Ev;
	while(1){
		if(x4*cos(x2*PI/180.0f) < 0){
			dir_flag = 1;	//正转
		}
		else{
			dir_flag = -1;	//反转
		}
		Ep = 0.004173f;
		Ev = 0.000356f*(x4*PI/180)*(x4*PI/180)+0.002082f*(1+cos(x2*PI/180.0f));
		//pwm = -dir_flag*(Ep-Ev)/Ep*PWM_GAIN;
		pwm = -dir_flag*20;
		//printf("x2=%f, x4=%f, dir_flag=%d, Ep=%f, Ev=%f, pwm=%f\r\n", x2, x4, dir_flag, Ep, Ev, pwm);
		delay_ms(100);
		motor_set_pwm(pwm);
	}
}

void starts_swinging(void)
{
	printf("starts_swinging\r\n");
}
void inverted_pendulum(void)
{
	printf("inverted_pendulum\r\n");
}
void circular_motion(void)
{
	printf("circular_motion\r\n");
}

