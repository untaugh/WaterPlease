#include "spislave.h"

static void (*spi_slave_read)(uint8_t);
static uint8_t (*spi_slave_write)(void);

#include "spislave.h"

void spi_init(void)
{
  // overflow interrupt, 3-wire, positive edge
  USICR = (1<<USIOIE) | (1<<USIWM0) | (1<<USICS1);
  
  // data out
  DDRB |= SPI_MISO;
  
  // clear interrupt flag, reset timer
  USISR = (1<<USIOIF);
  
  sei();
}

void spi_set_slave_read(void (*read_func)(uint8_t))
{
  spi_slave_read = read_func;
}
void spi_set_slave_write(uint8_t(*write_func)(void))
{
  spi_slave_write = write_func;

}

ISR(USI_OVF_vect)
{
  if (USIDR != 0)
    {
      // slave has recieved a command
      spi_slave_read(USIDR);
      
      // prepare to send first byte, in case command requires it
      USIDR = spi_slave_write();
    }
  else 
    {
      // slave writes data to master
      USIDR = spi_slave_write();
    }

  // clear interrupt flag, reset timer
  USISR = (1<<USIOIF);
}
