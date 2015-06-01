#ifndef RELAY_H
#define RELAY_H

#include <stdint.h>

enum RelayStatus
  {
    RELAY_NO_ERROR = 0,
    RELAY_ERROR_WATER_LOW,
    RELAY_ERROR_COOLDOWN,
    RELAY_ERROR_MAXON,
  };

// init relay with port and pin
void relay_init(volatile uint8_t * port, uint8_t pin);

// start relay
void relay_set_on(void);

// stop relay
void relay_set_off(void);

// return true if relay is on
uint8_t relay_running(void);

// get relay status
uint8_t relay_get_status(void);

// set cooldown of relay
void relay_set_cooldown(uint32_t time);

// get relay cooldown time
uint32_t relay_get_cooldown(void);

// increment relay timer
void relay_timer_inc(void);

// set max relay on time
void relay_set_maxon(uint32_t time);

// get relay max on time
uint32_t relay_get_maxon(void);

// get relay timer
uint32_t relay_get_timer(void);

// clear errors
void relay_clear_error(void);

#endif // RELAY_H
