#ifndef SPISLAVE_H
#define SPISLAVE_H

#include <avr/io.h>
#include <avr/interrupt.h>

#define SPI_MOSI (1<<PB0)
#define SPI_MISO (1<<PB1)
#define SPI_CLK (1<<PB2)

// init spi
void spi_init(void);

// set function pointer to slave read
void spi_set_slave_read(void (*)(uint8_t));

// set function pointer to slave write
void spi_set_slave_write(uint8_t(*)(void));

#endif // SPISLAVE_H
