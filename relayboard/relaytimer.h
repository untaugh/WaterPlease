#ifndef RELAYTIMER_H
#define RELAYTIMER_H

#include <stdint.h>

// clear timer
void timer_clear(void);

// increment timer
void timer_inc(void);

// set timer value
void timer_set(uint32_t);

// get timer value
uint32_t timer_get(void);

#endif // RELAYTIMER_H
