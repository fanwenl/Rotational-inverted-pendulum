#include "spi.h"
#include "util.h"
#include "touch.h"
#include "bitband.h"
#include "lcd.h"


void spi_portInit(void)
{
	portOutInit(GPIOB, 
				GPIO_Pin_4,
				GPIO_Mode_AF,
				GPIO_OType_PP,
				GPIO_PuPd_UP,
				GPIO_Speed_50MHz);
	
	portOutInit(GPIOA, 
				GPIO_Pin_5|GPIO_Pin_7,
				GPIO_Mode_AF,
				GPIO_OType_PP,
				GPIO_PuPd_UP,
				GPIO_Speed_50MHz);
	portOutInit(GPIOA, 
				GPIO_Pin_3,
				GPIO_Mode_OUT,
				GPIO_OType_PP,
				GPIO_PuPd_UP,
				GPIO_Speed_50MHz);
	portInInit(GPIOC, 
			   GPIO_Pin_5,
			   GPIO_Mode_IN,
			   GPIO_PuPd_UP);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);
}

void spi_configuration(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	
	spi_portInit();
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//串行同步时钟的空闲状态为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//串行同步时钟的第一个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	T_CS = 0;
	//spi_readWriteByte(0xff);//启动传输	
}

u8 spi_readWriteByte(u8 txData)
{			
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
	SPI_I2S_SendData(SPI1, txData); //通过外设SPIx发送一个byte  数据
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //等待接收完一个byte  
	return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据	
}


