#include "CppUTest/TestHarness.h"
#include "water.h"

#define WATER_PIN 3

TEST_GROUP(water)
{
  void setup()
  {
    water_init(&water_port, WATER_PIN);
    water_port |= (1<<WATER_PIN); // start high = got water
  }
  
  void teardown()
  {
  }

  uint8_t water_port;
  
};

TEST(water, WaterMemory)
{
  water_port = 0;

  CHECK_TRUE(water_low());

  water_port = (1<<WATER_PIN);
  
  CHECK_FALSE(water_low());
}
