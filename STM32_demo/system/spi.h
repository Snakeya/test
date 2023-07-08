#ifndef __SPI_H
#define __SPI_H

void soft_spi_start(void);
void soft_spi_end(void);
void soft_spi_init(void);
uint8_t soft_spi_swap_byte(uint8_t sendbyte);




#endif