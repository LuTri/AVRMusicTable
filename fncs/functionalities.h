#ifndef _FUNCTIONALITIES_H
#define _FUNCTIONALITIES_H

#include <avr/io.h>
#include "../OdroidUart/commands.h"
#include "../globals.h"

void text_writer(COMMAND_BUFFER* command);

extern uint8_t loop_data[60];
#endif