#include "usart.h"
#include "util.h"
#include "gpio.h"

void usart_configuration(USART_TypeDef* usartx, uint32_t baudRate)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_TypeDef* gpio_txd;
	GPIO_TypeDef* gpio_rxd;
	uint16_t pinSource_txd;
	uint16_t pinSource_rxd;
	uint8_t gpio_af;
	uint32_t pin_txd;
	uint32_t pin_rxd;
	uint8_t irq_channel;
	
	if(usartx == USART1){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		gpio_txd = GPIOB;
		gpio_rxd = GPIOB;
		pinSource_txd = GPIO_PinSource6;
		pinSource_rxd = GPIO_PinSource7;
		gpio_af = GPIO_AF_USART1;
		pin_txd = GPIO_Pin_6;
		pin_rxd = GPIO_Pin_7;
		irq_channel = USART1_IRQn;
	}
	else if(usartx == USART2){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		gpio_txd = GPIOA;
		gpio_rxd = GPIOA;
		pinSource_txd = GPIO_PinSource2;
		pinSource_rxd = GPIO_PinSource3;
		gpio_af = GPIO_AF_USART2;
		pin_txd = GPIO_Pin_2;
		pin_rxd = GPIO_Pin_3;
		irq_channel = USART2_IRQn;
	}
	else if(usartx == USART3){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		gpio_txd = GPIOB;
		gpio_rxd = GPIOB;
		pinSource_txd = GPIO_PinSource10;
		pinSource_rxd = GPIO_PinSource11;
		gpio_af = GPIO_AF_USART3;
		pin_txd = GPIO_Pin_10;
		pin_rxd = GPIO_Pin_11;
		irq_channel = USART3_IRQn;
	}
	else if(usartx == UART4){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
		gpio_txd = GPIOC;
		gpio_rxd = GPIOC;
		pinSource_txd = GPIO_PinSource10;
		pinSource_rxd = GPIO_PinSource11;
		gpio_af = GPIO_AF_UART4;
		pin_txd = GPIO_Pin_10;
		pin_rxd = GPIO_Pin_11;
		irq_channel = UART4_IRQn;
	}
	else if(usartx == UART5){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
		gpio_txd = GPIOC;
		gpio_rxd = GPIOD;
		pinSource_txd = GPIO_PinSource12;
		pinSource_rxd = GPIO_PinSource2;
		gpio_af = GPIO_AF_UART5;
		pin_txd = GPIO_Pin_12;
		pin_rxd = GPIO_Pin_2;
		irq_channel = UART5_IRQn;
	}
	else{
		while(1);
	}
	
	GPIO_PinAFConfig(gpio_txd, pinSource_txd, gpio_af);
    GPIO_PinAFConfig(gpio_rxd, pinSource_rxd, gpio_af);
	
	gpio_af_pp_up_init(gpio_txd, pin_txd);
	gpio_af_pp_up_init(gpio_rxd, pin_rxd);
	
	USART_InitStructure.USART_BaudRate = baudRate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(usartx,&USART_InitStructure); 
	USART_ITConfig(usartx,USART_IT_RXNE,ENABLE);
	USART_Cmd(usartx,ENABLE);
	USART_ClearFlag(usartx, USART_FLAG_TC);
	
	nvic_configuration(irq_channel, 4);
}

int fputc(int ch, FILE *f)
{	
	USART_SendData(UART4, (uint8_t) ch);
	while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET);
	return ch;
}

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

void UART4_IRQHandler(void)
{
	if(USART_GetFlagStatus(UART4,USART_FLAG_RXNE)==SET)
	{
		USART_ReceiveData(UART4);
		USART_ClearITPendingBit(UART4,USART_IT_RXNE);

	}
}
