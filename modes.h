#ifndef _MODES_H
#define _MODES_H

#include "globals.h"
#include "hostcom/host.h"

void slave(void);
void mood(void);
void sound(void);
void white(void);
void off(void);
void byte(uint16_t value);

void boot(void);

void byte_offset(uint8_t value, uint8_t offset);

extern uint8_t mode;

#endif
