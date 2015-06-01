#include "relay.h"
#include "relaytimer.h"
#include "water.h"

static volatile uint8_t * relay_port = 0;
static uint8_t relay_pin = 0;
static uint32_t cooldown_time = 0;
static uint32_t maxon_time = 0;
static uint8_t error = 0;
static void set_error(uint8_t);

void relay_set_on(void)
{
  if (water_low())
    {
      // water level too low
      set_error(RELAY_ERROR_WATER_LOW);
    }
  else if (!relay_running())
    {
      // relay not running yet
      if (timer_get() < cooldown_time)
	{
	  // cooldown not reached yet
	  set_error(RELAY_ERROR_COOLDOWN);
	}
      else
	{
	  // turn relay on, if not yet running
	  *relay_port |= (uint8_t)(1<<relay_pin); // write to port
	  timer_clear(); // reset timer, start counting up
	}
    }
}

void relay_set_off(void)
{
  *relay_port &= (uint8_t)~(1<<relay_pin);
}

uint8_t relay_get_status(void)
{
  return error;
}

void relay_init(volatile uint8_t * port, uint8_t pin)
{
  relay_port = port;
  relay_pin = pin;

  // clear error flag
  relay_clear_error();
  
  // set timer to max value
  timer_set(-1);
}

void relay_set_cooldown(uint32_t time)
{
  cooldown_time = time;
}

void relay_timer_inc(void)
{
  timer_inc();
  
  if (water_low())
    {
      set_error(RELAY_ERROR_WATER_LOW);
      relay_set_off();
    }
  else if (relay_running() && timer_get() >= maxon_time)
    {
      set_error(RELAY_ERROR_MAXON);
      relay_set_off();
    }
}

void relay_set_maxon(uint32_t time)
{
  maxon_time  = time; 
}

uint32_t relay_get_cooldown()
{
  return cooldown_time;
}

uint32_t relay_get_maxon(void)
{
  return maxon_time;
}

uint32_t relay_get_timer(void)
{
  return timer_get();
}

uint8_t relay_running(void)
{
  if (*relay_port & (1<<relay_pin))
    {
      return 1;
    }
  else
    {
      return 0;
    }
}

// only set error if there isn't one already
static void set_error(uint8_t new_error)
{
  if (relay_get_status() == RELAY_NO_ERROR)
    {
      error = new_error;
    }
}

void relay_clear_error(void)
{
  error = RELAY_NO_ERROR;
}
