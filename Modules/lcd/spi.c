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
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�͵�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//����ͬ��ʱ�ӵĵ�һ�������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
	T_CS = 0;
	//spi_readWriteByte(0xff);//��������	
}

u8 spi_readWriteByte(u8 txData)
{			
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
	SPI_I2S_SendData(SPI1, txData); //ͨ������SPIx����һ��byte  ����
	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�������һ��byte  
	return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����	
}


