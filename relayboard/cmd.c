#include "cmd.h"
#include "relay.h"
#include "deviceid.h"

static uint8_t data[4];  // 4 bytes of data
static uint8_t data_index = 0;

static void set_data_int(uint32_t data_int)
{
  data[3] = (uint8_t)data_int;
  data[2] = (uint8_t)(data_int>>8);
  data[1] = (uint8_t)(data_int>>16);
  data[0] = (uint8_t)(data_int>>24);
}

void cmd_slave_read(uint8_t cmd)
{
  data_index = 0; 
  
  if (cmd == CMD_SET_ON)
    {
      relay_set_on();

      // clear data in case it is read
      set_data_int(0);
    }
  else if (cmd == CMD_SET_OFF)
    {
      relay_set_off();

      // clear data in case it is read
      set_data_int(0);
    }
  else if (cmd == CMD_GET_STATUS)
    {
      set_data_int(relay_get_status());
    }
  else if (cmd == CMD_GET_COOLDOWN)
    {
      set_data_int(relay_get_cooldown());
    }
  else if (cmd == CMD_GET_MAXON)
    {
      set_data_int(relay_get_maxon());
    }
  else if (cmd == CMD_GET_TIMER)
    {
      set_data_int(relay_get_timer());
    }
  else if (cmd == CMD_GET_PIN)
    {
      set_data_int(relay_running());
    }
  else if (cmd == CMD_GET_ID)
    {
      set_data_int(DEVICE_ID);
    }
  else if (cmd == CMD_CLEAR_ERROR)
    {
      relay_clear_error();
      
      // clear data in case it is read
      set_data_int(0);
    }
  else
    {
      // if no command match, clear data
      set_data_int(0);
    }
}

uint8_t cmd_slave_write(void)
{
  if (data_index < sizeof(data))
    {
      return data[data_index++];
    }
  else
    {
      return 0;
    }
}
