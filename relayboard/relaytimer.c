#include "relaytimer.h"

static uint32_t time; 

void timer_clear(void)
{
  time = 0;
}

void timer_inc(void)
{
  // don't increment top value
  if (time < (uint32_t)-1)
    {
      time++;
    }
}

void timer_set(uint32_t set_time)
{
  time = set_time;
}

uint32_t timer_get(void)
{
  return time;
}

