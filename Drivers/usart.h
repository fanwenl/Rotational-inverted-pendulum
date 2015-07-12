#ifndef __USART_H__
#define __USART_H__

#include "stm32f4xx.h"
#include <stdio.h>

void usart_configuration(USART_TypeDef* usartx, uint32_t baudRate);
void usart_sendByte(USART_TypeDef *usart, u8 val);
void usart_sendBytes(USART_TypeDef *usart, u8 *buffer, u16 len);

#endif
