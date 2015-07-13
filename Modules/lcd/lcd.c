#include "lcd.h"
#include "util.h"
#include "delay.h"
#include <stdio.h>

extern const u8 ASCII_1608[1520];

void lcd_init(void)
{
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	FSMC_NORSRAMTimingInitTypeDef  writeTiming;
	
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);
	
	portOutInit(GPIOB, 				//LCD������ƽ�
				GPIO_Pin_1, 
				GPIO_Mode_OUT, 
				GPIO_OType_PP, 
				GPIO_PuPd_UP, 
				GPIO_Speed_50MHz);
	portOutInit(GPIOF, 				//LCD��λ����
				GPIO_Pin_11, 
				GPIO_Mode_OUT, 
				GPIO_OType_PP, 
				GPIO_PuPd_UP, 
				GPIO_Speed_50MHz);
	LCD_LED_A = 1;
	LCD_RESET = 0;
	delay_ms(100);
	LCD_RESET = 1;
	delay_ms(100);
	
	portOutInit(GPIOD, 
				GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9
					|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15,
				GPIO_Mode_AF, 
				GPIO_OType_PP, 
				GPIO_PuPd_UP, 
				GPIO_Speed_100MHz);
	portOutInit(GPIOE, 
				(0X1FF<<7),	//PE7~15
				GPIO_Mode_AF, 
				GPIO_OType_PP, 
				GPIO_PuPd_UP, 
				GPIO_Speed_100MHz);
	portOutInit(GPIOF, 
				GPIO_Pin_12,//FSMC_A6,��LCD_RS
				GPIO_Mode_AF, 
				GPIO_OType_PP, 
				GPIO_PuPd_UP, 
				GPIO_Speed_100MHz);
	portOutInit(GPIOG, 
				GPIO_Pin_12,//FSMC_NE4,��LCD_CS
				GPIO_Mode_AF, 
				GPIO_OType_PP, 
				GPIO_PuPd_UP, 
				GPIO_Speed_100MHz);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);	//FSMC_D2
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);	//FSMC_D3
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC);	//FSMC_NOE
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC);	//FSMC_NWE
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC);	//FSMC_D13
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC);	//FSMC_D14
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC);	//FSMC_D15
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC);	//FSMC_D0
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);	//FSMC_D1

	GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);	//FSMC_D4
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);	//FSMC_D5
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);	//FSMC_D6
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);	//FSMC_D7
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC);	//FSMC_D8
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC);	//FSMC_D9
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC);	//FSMC_D10
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC);	//FSMC_D11
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC);	//FSMC_D12

	GPIO_PinAFConfig(GPIOF,GPIO_PinSource12,GPIO_AF_FSMC);	//FSMC_A6
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource12,GPIO_AF_FSMC);	//FSMC_NE4
	
	readWriteTiming.FSMC_AddressSetupTime = 30;
	readWriteTiming.FSMC_AddressHoldTime = 0x00;
	readWriteTiming.FSMC_DataSetupTime = 30;
	readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	readWriteTiming.FSMC_CLKDivision = 0x00;
	readWriteTiming.FSMC_DataLatency = 0x00;
	readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;//ģʽA 

	writeTiming.FSMC_AddressSetupTime =10;
	writeTiming.FSMC_AddressHoldTime = 0x00;
	writeTiming.FSMC_DataSetupTime = 10;
	writeTiming.FSMC_BusTurnAroundDuration = 0x00;
	writeTiming.FSMC_CLKDivision = 0x00;
	writeTiming.FSMC_DataLatency = 0x00;
	writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;//ģʽA 
	
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;						//��������ʹ��NE4 ��Ҳ�Ͷ�ӦBTCR[6],[7]��
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; 	//���������ݵ�ַ
	FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;				//�洢������SRAM   
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;		//�洢�����ݿ��Ϊ16bit   
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;		//�洢��дʹ��
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;   
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; 		//��дʹ�ò�ͬ��ʱ��
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; 		//��дʱ��
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  				//дʱ��
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //��ʼ��FSMC����
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);  // ʹ��BANK1-SECTOR4
	
	delay_ms(100);
	
	printf("LCD device id = %x\r\n", lcd_read_reg(0x0000));
	lcd_write_reg_data(0x00E5,0x78F0);
	lcd_write_reg_data(0x0001,0x0100);
	lcd_write_reg_data(0x0002,0x0700);
	lcd_write_reg_data(0x0003,0x1030);
	lcd_write_reg_data(0x0004,0x0000); 
	lcd_write_reg_data(0x0008,0x0202);
	lcd_write_reg_data(0x0009,0x0000);
	lcd_write_reg_data(0x000A,0x0000);
	lcd_write_reg_data(0x000C,0x0000);
	lcd_write_reg_data(0x000D,0x0000);
	lcd_write_reg_data(0x000F,0x0000);
	//power on sequence VGHVGL
	lcd_write_reg_data(0x0010,0x0000);   
	lcd_write_reg_data(0x0011,0x0007);  
	lcd_write_reg_data(0x0012,0x0000);  
	lcd_write_reg_data(0x0013,0x0000); 
	delay_ms(20);
	lcd_write_reg_data(0x0007,0x0000); 
	//vgh 
	lcd_write_reg_data(0x0010,0x1690);   
	lcd_write_reg_data(0x0011,0x0227);
	delay_ms(20);
	//vregiout 
	lcd_write_reg_data(0x0012,0x009D); //0x001b
	delay_ms(20); 
	//vom amplitude
	lcd_write_reg_data(0x0013,0x1900);
	delay_ms(20); 
	//vom H
	lcd_write_reg_data(0x0029,0x0025); 
	delay_ms(20); 
	lcd_write_reg_data(0x002B,0x000D); 
	//gamma
	lcd_write_reg_data(0x0030,0x0007);
	lcd_write_reg_data(0x0031,0x0303);
	lcd_write_reg_data(0x0032,0x0003);//0006
	lcd_write_reg_data(0x0035,0x0206);
	lcd_write_reg_data(0x0036,0x0008);
	lcd_write_reg_data(0x0037,0x0406); 
	lcd_write_reg_data(0x0038,0x0304);//0200
	lcd_write_reg_data(0x0039,0x0007); 
	lcd_write_reg_data(0x003C,0x0602);//0504
	lcd_write_reg_data(0x003D,0x0008); 
	//ram
	lcd_write_reg_data(0x0050,0x0000);
	lcd_write_reg_data(0x0051,0x00EF);
	lcd_write_reg_data(0x0052,0x0000);
	lcd_write_reg_data(0x0053,0x013F);
	lcd_write_reg_data(0x0060,0xA700);
	lcd_write_reg_data(0x0061,0x0001);
	lcd_write_reg_data(0x006A,0x0000);

	lcd_write_reg_data(0x0080,0x0000);
	lcd_write_reg_data(0x0081,0x0000);
	lcd_write_reg_data(0x0082,0x0000);
	lcd_write_reg_data(0x0083,0x0000);
	lcd_write_reg_data(0x0084,0x0000);
	lcd_write_reg_data(0x0085,0x0000);

	lcd_write_reg_data(0x0090,0x0010);
	lcd_write_reg_data(0x0092,0x0600);

	lcd_write_reg_data(0x0007,0x0133);
	lcd_write_reg_data(0x00,0x0022);
}

void lcd_write_reg(u16 reg)
{
	reg=reg;			//ʹ��-O2�Ż���ʱ��,����������ʱ
	LCD->LCD_REG=reg;
}

void lcd_write_data(u16 dat)
{
	dat=dat;			//ʹ��-O2�Ż���ʱ��,����������ʱ
	LCD->LCD_RAM=dat;	
}

void lcd_write_reg_data(u16 reg, u16 dat)
{
	lcd_write_reg(reg);
	lcd_write_data(dat);
}

u16 lcd_read_data(void)
{
	vu16 ram;			//��ֹ���Ż�
	ram=LCD->LCD_RAM;
	return ram;
}

u16 lcd_read_reg(u16 reg)
{
	lcd_write_reg(reg);
	delay_us(5);
	return lcd_read_data();
}

void lcd_prepare_write_ram(void)
{
	LCD->LCD_REG = 0X0022;
}

void lcd_write_ram(u16 color)
{
	color = color;
	LCD->LCD_RAM = color;
}

void lcd_set_cursor(u16 x, u16 y)
{
	lcd_write_reg_data(0X0020, x);
	lcd_write_reg_data(0X0021, y);
}

void opt_delay(u8 n)
{
	while(n--);
}

u16 lcd_read_point_color(u16 x, u16 y)
{
	lcd_set_cursor(x, y);
	lcd_write_reg(0X0022);
	lcd_read_data();	//���
	opt_delay(2);
	return lcd_read_data();
}

void lcd_display_on(void)
{
	lcd_write_reg_data(0X0007, 0X0173);
}

void lcd_display_off(void)
{
	lcd_write_reg_data(0X0007, 0X0000);
}

void lcd_set_window(u16 x1, u16 y1, u16 x2, u16 y2)
{
	lcd_write_reg_data(0x0050, x1);
	lcd_write_reg_data(0x0052, y1);
	lcd_write_reg_data(0x0051, x2);
	lcd_write_reg_data(0x0053, y2);
	lcd_set_cursor(x1, y1);
	lcd_write_reg(0x0022);
}

void lcd_clear(u16 color)
{
	u32 i = 0;
	lcd_set_window(0, 0, LCD_W-1, LCD_H-1);
	for(i=0; i<LCD_W*LCD_H; i++){
		lcd_write_data(color);
	}
}

void lcd_draw_point(u16 x, u16 y, u16 color)
{
	lcd_set_window(x, y, x, y);
	lcd_write_data(color);
}

void lcd_fill(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd, u16 color)
{
	u32 i = 0;
	lcd_set_window(xStart, yStart, xEnd, yEnd);
	for(i=0; i<(xEnd-xStart+1)*(yEnd-yStart+1); i++){
		lcd_write_data(color);
	}
}

void lcd_draw_big_point(u16 x, u16 y, u16 color)
{
	lcd_fill(x-1, y-1, x+1, y+1, color);
}

void lcd_draw_line(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	u16 t; 
	int xerr=0, yerr=0, delta_x, delta_y, distance; 
	int incx, incy, uRow, uCol; 

	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0){
		incx=1; //���õ������� 
	}
	else if(delta_x==0){
		incx=0;//��ֱ�� 
	}
	else{
		incx=-1;delta_x=-delta_x;
	} 
	
	if(delta_y>0){
		incy=1; 
	}
	else if(delta_y==0){
		incy=0;//ˮƽ�� 
	}
	else{
		incy=-1;delta_y=-delta_y;
	} 
	if(delta_x>delta_y){
		distance=delta_x; //ѡȡ��������������
	}		
	else{
		distance=delta_y; 
	}
	for(t=0; t<=distance+1; t++){	//�������   
		lcd_draw_point(uRow,uCol, color);//���� 
		xerr+=delta_x ;
		yerr+=delta_y ;
		if(xerr>distance){ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance){ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	} 
}

void lcd_draw_rectangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)
{
	lcd_draw_line(x1,y1,x2,y1, color);
	lcd_draw_line(x1,y1,x1,y2, color);
	lcd_draw_line(x1,y2,x2,y2, color);
	lcd_draw_line(x2,y1,x2,y2, color);
}

void lcd_draw_circle(u16 x0, u16 y0, u8 r, u16 color)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		lcd_draw_point(x0-b, y0-a, color);             //3           
		lcd_draw_point(x0+b, y0-a, color);             //0           
		lcd_draw_point(x0-a, y0+b, color);             //1       
		lcd_draw_point(x0-b, y0-a, color);             //7           
		lcd_draw_point(x0-a, y0-b, color);             //2             
		lcd_draw_point(x0+b, y0+a, color);             //4               
		lcd_draw_point(x0+a, y0-b, color);             //5
		lcd_draw_point(x0+a, y0+b, color);             //6 
		lcd_draw_point(x0-b, y0+a, color);             
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0){
			di += 4*a+6;	  
		}
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		lcd_draw_point(x0+a, y0+b, color);
	}
}

void lcd_show_char(u16 x, u16 y, u8 num, u16 color)
{
    u8 temp;
    u8 pos,t;
	u16 x0;
	x++;
	x0=x;     
    if(x > LCD_W-16 || y > LCD_H-16){
		return;
	}
	num=num-' ';							 //�õ�ƫ�ƺ��ֵ
	for(pos=0; pos<16; pos++)
	{ 
		temp = ASCII_1608[(u16)num*16+pos];	 //����1608����
		for(t=0; t<8; t++)
		{                 
			if(temp & 0x01){
				lcd_draw_big_point(x+t, y+pos, color);
			}
			temp >>= 1; 
			x++;
		}
		x = x0;
		y++;
	}		   	 	  
}

void lcd_show_str(u16 x, u16 y, const char *p, u16 color)
{         
    while(*p!='\0')
    {
        if(x > LCD_W-16){
			x=0;
			y += 32;
		}
        if(y > LCD_H-16){
			y = x = 0;
			lcd_clear(RED);
		}
        lcd_show_char(x, y, *p, color);
        x += 16;
        p++;
    }  
}

void lcd_show_num(u16 x, u16 y, u32 num, u16 color)
{
	char str[10] = {0};
	sprintf(str, "%d", num);
	lcd_show_str(x, y, str, color);
}


