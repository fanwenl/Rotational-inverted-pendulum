#ifndef __SPI_H__
#define __SPI_H__

#include "stm32f4xx.h"
#include "bitband.h"

#define SPI1_CS		PaOutBit(1)
#define SPI2_CS		PaOutBit(2)
#define SPI3_CS		PaOutBit(3)

void spi_configuration(SPI_TypeDef *spix);
u8 spi_read_write_byte(SPI_TypeDef *spix, u8 txData);

#endif
