#ifndef WATER_H
#define WATER_H

#include <stdint.h>

// init water sensor
void water_init(volatile uint8_t * pins, uint8_t pin);

// is water level too low
uint8_t water_low(void);

#endif // WATER_H
