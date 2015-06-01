#include <avr/io.h>
#include <avr/interrupt.h>
#include "relayboard.h"
#include "relay.h"
#include "cmd.h"
#include "spislave.h"
#include "water.h"

//#define TIMER_TICK 30.517578125
#define TIMER_TICK 32UL
#define TIMER_SECONDS(s) (uint32_t)(s*TIMER_TICK)
#define TIMER_MINUTES(m) (uint32_t)(m*TIMER_TICK*60UL)
#define TIMER_HOURS(h) (uint32_t)(h*TIMER_TICK*60UL*60UL)
#define TIMER_DAYS(d) (uint32_t)(d*TIMER_TICK*60UL*60UL*24UL)
#define TIMER_WEEKS(w) (uint32_t)(w*TIMER_TICK*60UL*60UL*24UL*7UL)

int main()
{
  // init water sensor
  WATER_LOW_PORT |= (1<<WATER_LOW); // enable pull-up
  water_init(&WATER_LOW_PIN, WATER_LOW);
  
  // init relay
  RELAY_DDR |= (1<<RELAY);
  relay_init(&RELAY_PORT, RELAY);
  relay_set_cooldown(TIMER_HOURS(1));
  relay_set_maxon(TIMER_MINUTES(31));
  
  // init timer interrupt
  TCCR0B |= (1<<CS02) | (1<<CS00);
  TIMSK |= (1<<TOIE0);
  
  // init spi
  spi_init();
  spi_set_slave_read(cmd_slave_read);
  spi_set_slave_write(cmd_slave_write);

  sei();
  
  while (1)
    { 
      
    }
}

// prescaler 1024 gives 30.5Hz
ISR(TIMER0_OVF_vect)
{
  // increment relay timer
  relay_timer_inc(); 
}
