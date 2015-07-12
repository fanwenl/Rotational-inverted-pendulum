#include "spi.h"
#include "gpio.h"
#include "usart.h"

//B3->SCK, B4->MISO, B5->MOSI, A1->CS
void spi1_portInit(void)
{
	gpio_af_pp_up_init(GPIOB, GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5);
	gpio_out_pp_up_init(GPIOA, GPIO_Pin_1);

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);
}

//B10->SCK, C2->MISO, C3->MOSI, A2->CS 
void spi2_portInit(void)
{
	gpio_af_pp_up_init(GPIOB, GPIO_Pin_10);
	gpio_af_pp_up_init(GPIOC, GPIO_Pin_2 | GPIO_Pin_3);
	gpio_out_pp_up_init(GPIOA, GPIO_Pin_2);

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource2,GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource3,GPIO_AF_SPI2);
}

void spi_configuration(SPI_TypeDef *spix)
{
	SPI_InitTypeDef  SPI_InitStructure;
	
	if(spix == SPI1){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		spi1_portInit();
	}
	else if(spix == SPI2){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
		spi2_portInit();
	}
	else{
		printf("spix is not supported!\r\n");
		while(1);
	}
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	//8位帧
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;			//clk空闲为低
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;		//clk第一个跳变沿采集数据
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;			//NSS信号由软件控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;			//CRC值计算的多项式
	SPI_Init(spix, &SPI_InitStructure);  
	SPI_Cmd(spix, ENABLE);
}

u8 spi_read_write_byte(SPI_TypeDef *spix, u8 txData)
{			
	while (SPI_I2S_GetFlagStatus(spix, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(spix, txData); 
	while (SPI_I2S_GetFlagStatus(spix, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(spix);
}

void spi_set_speed(SPI_TypeDef *spix, u8 speed)
{
	spix->CR1 &= 0XFFC7; 
	spix->CR1 |= speed;
	SPI_Cmd(spix,ENABLE); 
}

