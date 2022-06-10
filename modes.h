#ifndef _MODES_H
#define _MODES_H

#include "globals.h"
#include "OdroidUart/commands.h"

void handle_data(void);
void main_loop(void);

void snake(COMMAND_BUFFER* command);
void slave(COMMAND_BUFFER* command);
void sound(void);
void white(void);
void red(void);
void green(void);
void blue(void);
void off(void);
void byte(uint16_t value);
void indicate(uint8_t pos, uint8_t r, uint8_t g, uint8_t b);
void indicate_t(uint8_t pos, uint8_t r, uint8_t g, uint8_t b);
void display_byte(uint8_t row, uint8_t byte, uint8_t do_set_leds);

void noop(void);

void boot(void);

void byte_offset(uint8_t value, uint8_t offset);

extern uint8_t mode;


#endif
