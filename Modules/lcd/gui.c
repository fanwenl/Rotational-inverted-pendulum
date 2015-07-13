#include "gui.h"
#include "lcd.h"

void gui_drawIcon(u16 x, u16 y, char *name, u16 color)
{
	lcd_draw_rectangle(x-32, y-48, x+32, y+16, color);
	lcd_draw_circle(x, y-16, 24, color);
	lcd_draw_line(x, y-48, x, y+16, color);
	lcd_draw_line(x-32, y-16, x+32, y-16, color);
	lcd_show_str(x-32, y+16, name, GRAYBLUE);
}

void gui_drawHome(void)
{
	lcd_draw_line(0, 290, 239, 290, BLUE);
	lcd_show_str(88, 290, "HOME", BLUE);
	lcd_show_str(APP1_X, APP1_Y, "Swinging      around", GRAYBLUE);
	lcd_draw_rectangle(APP1_X-20, APP1_Y, APP1_X+140, APP1_Y+65, GRAYBLUE);
	lcd_show_str(APP2_X, APP2_Y, "starts        swinging", GRAYBLUE);
	lcd_draw_rectangle(APP2_X-20, APP2_Y, APP2_X+140, APP2_Y+65, GRAYBLUE);
	lcd_show_str(APP3_X, APP3_Y, "Inverted      Pendulum", GRAYBLUE);
	lcd_draw_rectangle(APP3_X-20, APP3_Y, APP3_X+140, APP3_Y+65, GRAYBLUE);
	lcd_show_str(APP4_X, APP4_Y, "Circular      motion", GRAYBLUE);
	lcd_draw_rectangle(APP4_X-20, APP4_Y, APP4_X+140, APP4_Y+65, GRAYBLUE);
	
}

u8 isInRange(u16 xPos, u16 yPos, u16 appx, u16 appy)
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
	lcd_clear(RED);
}

void starts_swinging(void)
{
	lcd_clear(BLUE);
}
void inverted_pendulum(void)
{
	lcd_clear(YELLOW);
}
void circular_motion(void)
{
	lcd_clear(GREEN);
}

