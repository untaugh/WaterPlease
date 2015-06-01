#include "CppUTest/TestHarness.h"
#include "relaytimer.h"

TEST_GROUP(relaytimer)
{
  void setup()
  {
    timer_clear();
  }
  void teardown()
  {
  }
};

TEST(relaytimer, TimeTestSimple)
{
  LONGS_EQUAL(0,timer_get());
  timer_inc();
  LONGS_EQUAL(1,timer_get());
  timer_clear();
  LONGS_EQUAL(0,timer_get());
}

TEST(relaytimer, TimeSet)
{
  timer_set(3267);
  LONGS_EQUAL(3267,timer_get());
}

TEST(relaytimer, IncrementMax)
{
  timer_set(-1);
  timer_inc();
  LONGS_EQUAL((uint32_t)-1,timer_get());
}
