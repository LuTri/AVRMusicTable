#include "functionalities.h"

void slave(COMMAND_BUFFER* command) {
    for (uint16_t idx = 0; idx < N_LEDS; idx++) {
        if (idx < command->size) {
            ((uint8_t*)&leds)[idx] = command->data[idx];
        } else {
            ((uint8_t*)&leds)[idx] = 0;
        }
    }

    if (command->size) {
        ws2812_setleds();
    } else {
        OFF()
    }
    //light_uart_status();

    loop_fnc = 0;
}
