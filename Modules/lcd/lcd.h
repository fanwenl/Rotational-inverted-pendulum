#ifndef __LCD_H__
#define __LCD_H__

#include "stm32f4xx.h"
#include "bitband.h"

//定义LCD的尺寸	
#define LCD_W 240
#define LCD_H 320

#define LCD_LED_A	PbOutBit(1)
#define LCD_RESET	PfOutBit(11)

typedef struct
{
	u16 LCD_REG;
	u16 LCD_RAM;
} LCD_TypeDef;			    
#define LCD_BASE	((u32)(0x6C000000 | 0x0000007E))
#define LCD			((LCD_TypeDef *) LCD_BASE)

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色 

#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

void lcd_init(void);

void lcd_write_reg(u16 reg);
void lcd_write_data(u16 dat);
void lcd_write_reg_data(u16 reg, u16 dat);
u16 lcd_read_data(void);
u16 lcd_read_reg(u16 reg);
void lcd_prepare_write_ram(void);
void lcd_write_ram(u16 color);
void lcd_set_cursor(u16 x, u16 y);
u16 lcd_read_point_color(u16 x, u16 y);
void lcd_display_on(void);
void lcd_display_off(void);
void lcd_set_window(u16 x1, u16 y1, u16 x2, u16 y2);

void lcd_clear(u16 color);
void lcd_draw_point(u16 x, u16 y, u16 color);
void lcd_fill(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd, u16 color);
void lcd_draw_big_point(u16 x, u16 y, u16 color);
void lcd_draw_line(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void lcd_draw_rectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void lcd_draw_circle(u16 x0, u16 y0, u8 r, u16 color);
void lcd_show_char(u16 x, u16 y, u8 num, u16 color);
void lcd_show_str(u16 x, u16 y, const char *p, u16 color);
void lcd_show_num(u16 x, u16 y, u32 num, u16 color);

#endif
