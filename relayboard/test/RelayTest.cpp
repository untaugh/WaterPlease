#include "CppUTest/TestHarness.h"
#include "relay.h"
#include "water.h"

#define RELAY_PIN 1
#define WATER_PIN 7

TEST_GROUP(relay)
{
  void setup()
  {
    relay_init(&relay_port, RELAY_PIN);
    relay_set_maxon(0xFFFFFFFF);
    water_init(&relay_port, WATER_PIN);  // same port as relay
    relay_port |= (1<<WATER_PIN); // start high = got water
  }
  
  void teardown()
  {
  }

  uint8_t relay_port;
  
};

// starting stopping 1
TEST(relay, RelayStartStop)
{
  relay_set_on();
  CHECK_TRUE(relay_running());
  relay_set_off();
  CHECK_FALSE(relay_running());
}

// starting stopping 2
TEST(relay, RelayMemory)
{
  relay_port = 0;
  CHECK_FALSE(relay_running());
  relay_port = (1<<RELAY_PIN);
  CHECK_TRUE(relay_running());
}

// starting stopping 3
TEST(relay, RelayMemory2)
{
  relay_set_on();
  CHECK_TRUE(relay_port & (1<<RELAY_PIN));
  relay_set_off();
  CHECK_FALSE(relay_port & (1<<RELAY_PIN));
}

// test cooldown error
TEST(relay, CooldownTest)
{
  relay_set_cooldown(100);
  relay_set_on();
  CHECK_TRUE(relay_running());
  relay_set_off();
  relay_set_on();
  BYTES_EQUAL(RELAY_ERROR_COOLDOWN, relay_get_status());
  CHECK_FALSE(relay_running());
}

// multiple starts, no error
TEST(relay, ManyStartsCooldown)
{
  relay_set_cooldown(100);
  relay_set_on();
  relay_set_on();
  relay_set_on();
  CHECK_TRUE(relay_running());
  BYTES_EQUAL(RELAY_NO_ERROR, relay_get_status());

}

// start despite error
TEST(relay,StartOnError)
{
  relay_set_cooldown(10);
  relay_set_on();
  CHECK_TRUE(relay_running());
  BYTES_EQUAL(RELAY_NO_ERROR, relay_get_status());
  relay_set_off();
  relay_set_on();
  BYTES_EQUAL(RELAY_ERROR_COOLDOWN, relay_get_status());
  CHECK_FALSE(relay_port & (1<<RELAY_PIN));
  for (int i=0; i<15;i++)
    {
      relay_timer_inc();
    }
  
  // relay should start despite having this error
  relay_set_on();
  BYTES_EQUAL(RELAY_ERROR_COOLDOWN, relay_get_status());
  CHECK_TRUE(relay_running());
  CHECK_TRUE(relay_port & (1<<RELAY_PIN));
}

// test max on time error
TEST(relay, MaximumOnTime)
{
  relay_set_maxon(10);

  relay_set_on();
  
  for (int i=0; i<9;i++)
    {
      relay_timer_inc();
    }
  CHECK_TRUE(relay_running());
  relay_timer_inc();
  BYTES_EQUAL(RELAY_ERROR_MAXON, relay_get_status());
  CHECK_FALSE(relay_running());
}

// test max on with one tick
TEST(relay, MaxOnOneCount)
{
  relay_set_maxon(1);

  relay_set_on();
  
  CHECK_TRUE(relay_running());
  relay_timer_inc();
  BYTES_EQUAL(RELAY_ERROR_MAXON, relay_get_status());
  CHECK_FALSE(relay_running());
}

// read cooldown
TEST(relay, getCooldown)
{
  relay_set_cooldown(21312);
  LONGS_EQUAL(21312,relay_get_cooldown());
  relay_set_cooldown(0xFFFFFFFF);
  LONGS_EQUAL(0xFFFFFFFF,relay_get_cooldown());
}

// read maxon
TEST(relay, getMaxon)
{
  relay_set_maxon(321312);
  LONGS_EQUAL(321312,relay_get_maxon());
  relay_set_maxon(0xFFFFFFFF);
  LONGS_EQUAL(0xFFFFFFFF,relay_get_maxon());
}

// read timer
TEST(relay, getTimer)
{
  relay_set_on();
  LONGS_EQUAL(0,relay_get_timer());

  for (int i=0; i<2050000; i++)
    {
      relay_timer_inc();
    }
  
  LONGS_EQUAL(2050000,relay_get_timer());
}

// test water level at start of relay on
TEST(relay, WaterErrorStart)
{
  relay_port &= (uint8_t)~(1<<WATER_PIN); // water too low

  relay_set_on();

  CHECK_FALSE(relay_port & (1<<RELAY_PIN));
  
  BYTES_EQUAL(RELAY_ERROR_WATER_LOW, relay_get_status());
  
}

// test water level goes low while relay on
TEST(relay, WaterErrorRunning)
{
  relay_set_on();

  for (int i=0; i<10; i++)
    {
      relay_timer_inc();
    }
  
  CHECK_TRUE(relay_running());

  relay_port &= (uint8_t)~(1<<WATER_PIN); // water too low

  relay_timer_inc();
  
  BYTES_EQUAL(RELAY_ERROR_WATER_LOW, relay_get_status());

  CHECK_FALSE(relay_running());
  
  CHECK_FALSE(relay_port & (1<<RELAY_PIN));
}

// max on time, but relay is off
TEST(relay, MaxonWhileoff)
{
  relay_set_on();
  relay_set_maxon(15);

  for (int i=0; i<10; i++)
    {
      relay_timer_inc();
    }

  relay_set_off();
    
  for (int i=0; i<10; i++)
    {
      relay_timer_inc();
    }
  CHECK_FALSE(relay_running());
  BYTES_EQUAL(RELAY_NO_ERROR, relay_get_status());
}

// try overwrite error, don't overwrite error message
TEST(relay, ErrorOverwrite)
{
  relay_set_on();
  relay_set_maxon(10);

  for (int i=0; i<15; i++)
    {
      relay_timer_inc();
    }

  BYTES_EQUAL(RELAY_ERROR_MAXON, relay_get_status());

  relay_port &= (uint8_t)~(1<<WATER_PIN); // water too low

  relay_timer_inc();

  BYTES_EQUAL(RELAY_ERROR_MAXON, relay_get_status());

  
}

// clear errors
TEST(relay, ClearErrors)
{
  relay_set_on();
  relay_set_maxon(10);

  for (int i=0; i<15; i++)
    {
      relay_timer_inc();
    }

  BYTES_EQUAL(RELAY_ERROR_MAXON, relay_get_status());
  relay_clear_error();
  BYTES_EQUAL(RELAY_NO_ERROR, relay_get_status());

}
