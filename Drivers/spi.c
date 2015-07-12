#include "spi.h"
#include "gpio.h"

//B10->SCK, C2->MISO, C3->MOSI, A2->CS
void spi2_config(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	gpio_af_pp_up_init(GPIOB, GPIO_Pin_10);
	gpio_af_pp_up_init(GPIOC, GPIO_Pin_2 | GPIO_Pin_3);
	gpio_out_pp_up_init(GPIOA, GPIO_Pin_2);	//Ƭѡ����
	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource2,GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource3,GPIO_AF_SPI2);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						//��ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					//8λ֡
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;							//clk����Ϊ��
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						//clk��һ�������زɼ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							//NSS�ź����������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;//������Ԥ��ƵֵΪ256,����
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;					//���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;							//CRCֵ����Ķ���ʽ
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

//SPI_BaudRatePrescaler_256:���٣� SPI_BaudRatePrescaler_8������
void spi2_set_speed(u8 speed)
{
	SPI2->CR1 &= 0XFFC7; 
	SPI2->CR1 |= speed;
	SPI_Cmd(SPI2,ENABLE); 
}

