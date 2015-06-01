#ifndef CMD_H
#define CMD_H

#include <stdint.h>

enum CmdCommands
  {
    CMD_VOID = 0,
    CMD_GET_ID,
    CMD_SET_ON,
    CMD_SET_OFF,
    CMD_GET_PIN,
    CMD_GET_STATUS,
    CMD_CLEAR_ERROR,
    CMD_GET_TIMER,
    CMD_GET_COOLDOWN,
    CMD_GET_MAXON,
  };

// slave read from master
void cmd_slave_read(uint8_t);

// slave write to master
uint8_t cmd_slave_write(void);

#endif // CMD_H
