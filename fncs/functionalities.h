#ifndef _FUNCTIONALITIES_H
#define _FUNCTIONALITIES_H

#include <avr/io.h>
#include "../OdroidUart/commands.h"
#include "../display/ws2812.h"
#include "../globals.h"

void rgb(uint8_t r, uint8_t g, uint8_t b);

#define WHITE() rgb(0xFF, 0xFF, 0xFF);
#define RED()   rgb(0xFF, 0x00, 0x00);
#define BLUE()  rgb(0x00, 0x00, 0xFF);
#define GREEN() rgb(0x00, 0xFF, 0x00);
#define OFF()   rgb(0x00, 0x00, 0x00);

void reboot(void);
void get_fuses(uint8_t* low, uint8_t* high, uint8_t* extended);

void text_writer(COMMAND_BUFFER* command);
void launch_mood(COMMAND_BUFFER* command);
void snake(COMMAND_BUFFER* command);
void slave(COMMAND_BUFFER* command);
void stl(COMMAND_BUFFER* command);

typedef void (*LOOP_PTR)(void);

extern LOOP_PTR loop_fnc;
extern uint8_t* loop_data;

extern uint8_t DO_BENCHMARK;

#endif