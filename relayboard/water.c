#include "water.h"

static volatile uint8_t * water_pins = 0;
static uint8_t water_pin = 0;


void water_init(volatile uint8_t * pins, uint8_t pin)
{
  water_pins = pins;
  water_pin = pin;
}


uint8_t water_low()
{
  // if pin is high
  if (*water_pins & (1<<water_pin))
    {
      return 0; // got water
    }
  else
    {
      return 1; // water low
    }
}
