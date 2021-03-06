#include "CppUTest/TestHarness.h"
#include "cmd.h"
#include "relay.h"
#include "water.h"

#define RELAY_PIN 1
#define WATER_PIN 3

TEST_GROUP(cmd)
{
  void setup()
  {
    relay_init(&gp_port, RELAY_PIN);
    relay_set_maxon(0xFFFFFFFF);
    water_init(&gp_port, WATER_PIN);  // same port as relay
    gp_port |= (1<<WATER_PIN); // start high = got water
  }
  
  void teardown()
  {
  }

  uint8_t gp_port;
  
  uint32_t read32()
  {
    uint32_t data;
    
    data = cmd_slave_write() << 24;
    data |= cmd_slave_write() << 16;
    data |= cmd_slave_write() << 8;
    data |= cmd_slave_write();

    return data;
  }

  void write8(uint8_t cmd)
  {
    cmd_slave_read(cmd);
  }
  
};

TEST(cmd, SetOnOff)
{
  write8(CMD_SET_ON);
  write8(CMD_GET_STATUS);
  LONGS_EQUAL(RELAY_NO_ERROR,read32());
  write8(CMD_GET_PIN);
  CHECK_TRUE(read32());

  
  cmd_slave_read(CMD_SET_OFF);
  cmd_slave_read(CMD_GET_STATUS);
  LONGS_EQUAL(RELAY_NO_ERROR,read32());
  write8(CMD_GET_PIN);
  CHECK_FALSE(read32());
}

TEST(cmd, CooldownMaxon)
{
  relay_set_cooldown(0xA5B4FF23);
  write8(CMD_GET_COOLDOWN);
  LONGS_EQUAL(0xA5B4FF23,read32());
}

TEST(cmd, Maxon)
{
  relay_set_maxon(0xB4F324A5);
  write8(CMD_GET_MAXON);
  LONGS_EQUAL(0xB4F324A5,read32());
}

TEST(cmd, ReadOver)
{
  write8(CMD_SET_ON);
  write8(CMD_GET_PIN);
  CHECK_TRUE(read32());

  for (int i=0; i<25; i++)
    {
      LONGS_EQUAL(0,read32());
    }
}

TEST(cmd, TestTimer)
{
  relay_set_on();
  write8(CMD_GET_TIMER);
  LONGS_EQUAL(0,read32());

  for (int i=0; i<33; i++)
    {
      relay_timer_inc();
    }
  write8(CMD_GET_TIMER);
  LONGS_EQUAL(33,read32());  
}

TEST(cmd, AbortCommand)
{
  write8(CMD_SET_ON);
  write8(CMD_GET_TIMER);
  write8(CMD_GET_STATUS);
  write8(CMD_GET_STATUS);
  LONGS_EQUAL(RELAY_NO_ERROR,read32());
}

TEST(cmd, MaxOnError)
{
  relay_set_maxon(10);
  write8(CMD_SET_ON);

  for (int i=0;i<11;i++ ) { 
    relay_timer_inc();    
  }

  write8(CMD_GET_STATUS);
  LONGS_EQUAL(RELAY_ERROR_MAXON,read32());
}


TEST(cmd, CooldownError)
{
  relay_set_cooldown(10);
  cmd_slave_read(CMD_SET_ON);
  cmd_slave_read(CMD_SET_OFF);
  cmd_slave_read(CMD_SET_ON);

  write8(CMD_GET_STATUS);
  LONGS_EQUAL(RELAY_ERROR_COOLDOWN,read32());
  write8(CMD_GET_PIN);
  CHECK_FALSE(read32());

}

TEST(cmd, WaterError)
{
  gp_port &= (uint8_t)~(1<<WATER_PIN); // water too low
  
  write8(CMD_SET_ON);

  write8(CMD_GET_STATUS);
  LONGS_EQUAL(RELAY_ERROR_WATER_LOW, read32());
}

TEST(cmd, GetId)
{
  write8(CMD_GET_ID);
  LONGS_EQUAL(133,read32());
}

TEST(cmd, UnknownCmd)
{
  write8(CMD_GET_ID);
  LONGS_EQUAL(133,read32());
  write8(99);
  LONGS_EQUAL(0,read32());
  write8(133);
  LONGS_EQUAL(0,read32());
}

// test no data after set on/off command
TEST(cmd, SetOnOffNoData)
{
  write8(CMD_GET_ID);
  LONGS_EQUAL(133,read32());
  write8(CMD_SET_ON);
  LONGS_EQUAL(0,read32());
  write8(CMD_SET_OFF);
  LONGS_EQUAL(0,read32());
}

// test clearing error
TEST(cmd, ClearError)
{
  relay_set_cooldown(10);
  cmd_slave_read(CMD_SET_ON);
  cmd_slave_read(CMD_SET_OFF);
  cmd_slave_read(CMD_SET_ON);

  write8(CMD_GET_STATUS);
  LONGS_EQUAL(RELAY_ERROR_COOLDOWN,read32());
  write8(CMD_GET_PIN);
  CHECK_FALSE(read32());

  write8(CMD_CLEAR_ERROR);
  write8(CMD_GET_STATUS);
  LONGS_EQUAL(RELAY_NO_ERROR,read32());

  cmd_slave_read(CMD_SET_ON);

  write8(CMD_GET_STATUS);
  LONGS_EQUAL(RELAY_ERROR_COOLDOWN,read32());

  
}
