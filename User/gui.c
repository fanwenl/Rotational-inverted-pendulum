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

void side_to_side(void)
{
	printf("side_to_side\r\n");
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

