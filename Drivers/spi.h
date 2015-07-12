#ifndef __SPI_H__
#define __SPI_H__

#include "stm32f4xx_spi.h"
#include "bitband.h"

#define SPI2_CS		PaOutBit(2)

void spi2_config(void);
u8 spi2_read_write_byte(u8 txData);
void spi2_set_speed(u8 speed);

#endif
