#include "usart.h"
#include "util.h"
#include "gpio.h"
#include "lqr.h"
#include "flash.h"

void usart1_init(uint32_t baudRate)
{
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);
	gpio_af_pp_up_init(GPIOB, GPIO_Pin_6|GPIO_Pin_7);
	
	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 
	USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);
	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
	nvic_config(USART1_IRQn, 4);
}

void uart4_init(uint32_t baudRate)
{
	USART_InitTypeDef USART_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);
	gpio_af_pp_up_init(GPIOC, GPIO_Pin_10|GPIO_Pin_11);
	
	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(UART4, &USART_InitStructure); 
	USART_ITConfig(UART4, USART_IT_RXNE,ENABLE);
	USART_Cmd(UART4, ENABLE);
	USART_ClearFlag(UART4, USART_FLAG_TC);
	
	nvic_config(UART4_IRQn, 7);
}

//重定向printf函数
int fputc(int ch, FILE *f)
{	
	USART_SendData(UART4, (uint8_t) ch);
	while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
	return ch;
}

////重定向scanf函数
//int fgetc(FILE *f)
//{
//	uint8_t val;
//	while (USART_GetFlagStatus(UART4, USART_FLAG_RXNE) == RESET);
//	val = USART_ReceiveData(UART4);
//	USART_SendData(UART4, val);
//	while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
//	return (int)val;
//}

void usart_sendByte(USART_TypeDef *usart, u8 val)
{
	USART_SendData(usart, val);
	while (USART_GetFlagStatus(usart, USART_FLAG_TC) == RESET);
}

void usart_sendBytes(USART_TypeDef *usart, u8 *buffer, u16 len)
{
	u16 i;
	for(i=0; i<len; i++){
		usart_sendByte(usart, buffer[i]);
	}
}

void USART1_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==SET)
	{
		USART_ReceiveData(USART1);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		
	}
}

void save_parameter(void)
{
	uint32_t parameter[5];
	parameter[0] = ((lqr_k1+1000)*1000+0.5f);
	parameter[1] = ((lqr_k2+1000)*1000+0.5f);
	parameter[2] = ((lqr_k3+1000)*1000+0.5f);
	parameter[3] = ((lqr_k4+1000)*1000+0.5f);
	parameter[4] = ((gain+1000)*1000+0.5f);
	flash_write_data32(parameter, 5);
}

#define HEAD1       0XAA
#define HEAD2_K1    0X11
#define HEAD2_K2    0X22
#define HEAD2_K3    0X33
#define HEAD2_K4    0X44
#define HEAD2_GAIN  0X55
#define HEAD2_SWITCH_START 0XBB
#define HEAD2_SWITCH_STOP  0XCC
#define NEG         0X01
#define POS         0X02
uint8_t start_flag = 0;
void UART4_IRQHandler(void)
{
	static uint8_t buffer[8];
	static uint8_t count = 0;
	float tmp;
	if(USART_GetFlagStatus(UART4,USART_FLAG_RXNE)==SET)
	{
		buffer[count] = USART_ReceiveData(UART4);
		while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
		USART_ClearITPendingBit(UART4,USART_IT_RXNE);
		
		if(buffer[0] != HEAD1 && count == 0)
			return;
		count++;
		if(count == 2 && buffer[1] != HEAD2_SWITCH_START && buffer[1] != HEAD2_SWITCH_STOP && buffer[1] != HEAD2_K1 && buffer[1] != HEAD2_K2 && buffer[1] != HEAD2_K3 && buffer[1] != HEAD2_K4 && buffer[1] != HEAD2_GAIN)
		{
			count = 0;
			return;
		}
		if(count == 8)
		{
			count = 0;
			if(buffer[7] != (buffer[2]^buffer[3]^buffer[4]^buffer[5]^buffer[6])){
				return;
			}
			if(buffer[1] == HEAD2_SWITCH_START){
				start_flag = 1;
				return;
			}
			else if(buffer[1] == HEAD2_SWITCH_STOP){
				start_flag = 0;
				return;
			}
			tmp = (float)(buffer[3]<<24|buffer[4]<<16|buffer[5]<<8|buffer[6])/1000.0f;
			tmp= (buffer[2]==POS)? tmp:-tmp;
			switch(buffer[1]){
				case HEAD2_K1:
					lqr_k1 = tmp;
					break;
				case HEAD2_K2:
					lqr_k2 = tmp;
					break;
				case HEAD2_K3:
					lqr_k3 = tmp;
					break;
				case HEAD2_K4:
					lqr_k4 = tmp;
					break;
				case HEAD2_GAIN:
					gain = tmp;
					break;
			}
		}
	}
}
