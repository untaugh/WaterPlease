#include "CppUTest/TestHarness.h"
#include "relay.h"
#include "water.h"

#define RELAY_PIN 2
#define WATER_PIN 5

TEST_GROUP(relayinit)
{
  void setup()
  {
    relay_init(&relay_port, RELAY_PIN);
    water_init(&relay_port, WATER_PIN);  // same port as relay
    relay_port |= (1<<WATER_PIN); // start high = got water
  }
  
  void teardown()
  {
  }

  uint8_t relay_port;
  
};

// inc timer before relay is on
TEST(relayinit, RelayStartStop)
{
  relay_set_maxon(30);
  relay_timer_inc();
  relay_set_on();
  CHECK_TRUE(relay_running());
  relay_set_off();
  CHECK_FALSE(relay_running());
  BYTES_EQUAL(RELAY_NO_ERROR, relay_get_status());
}
