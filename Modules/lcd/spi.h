#ifndef __SPI_H__
#define __SPI_H__

#include "stm32f4xx.h"

void spi_portInit(void);
void spi_configuration(void);
u8 spi_readWriteByte(u8 txData);

#endif
