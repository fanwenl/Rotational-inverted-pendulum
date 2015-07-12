#include "spi.h"
#include "gpio.h"

//B10->SCK, C2->MISO, C3->MOSI, A2->CS
void spi2_config(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	gpio_af_pp_up_init(GPIOB, GPIO_Pin_10);
	gpio_af_pp_up_init(GPIOC, GPIO_Pin_2 | GPIO_Pin_3);
	gpio_out_pp_up_init(GPIOA, GPIO_Pin_2);	//片选引脚
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource2,GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource3,GPIO_AF_SPI2);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						//主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					//8位帧
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							//clk空闲为低
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						//clk第一个跳变沿采集数据
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							//NSS信号由软件控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//波特率预分频值为256,低速
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					//数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;							//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);
	
	SPI2_CS = 0;
}

u8 spi2_read_write_byte(u8 txData)
{			
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI2, txData); 
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPI2);
}

//SPI_BaudRatePrescaler_256:低速， SPI_BaudRatePrescaler_8：高速
void spi2_set_speed(u8 speed)
{
	SPI2->CR1 &= 0XFFC7; 
	SPI2->CR1 |= speed;
	SPI_Cmd(SPI2,ENABLE); 
}

