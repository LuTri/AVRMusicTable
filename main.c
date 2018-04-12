#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "globals.h"
#include "modes.h"

#include "AVRClock/customtimer.h"

#include <util/delay.h>

int main(void) {
    T_CALLBACK functions[] = {
        white, red, off, green, off,
        white, red, off, green, off,
    };
    T_CALLBACK current = NULL;
    float seconds[10] = {0.5,1.0,2.0,4.0,8.0,16.0,32.0,64.0,128.0,300.0};
    uint8_t in_isr[10] = {0};
    uart_init();
    char buffer[100];
    uint8_t idx = 0;

    uart_puts("BOOTING MC\n");
    sprintf(buffer, "RED is %p\n", red);
    uart_puts(buffer);
    sprintf(buffer, "WHITE is %p\n", white);
    uart_puts(buffer);
    sprintf(buffer, "GREEN is %p\n", green);
    uart_puts(buffer);
    sprintf(buffer, "BLUE is %p\n", blue);
    uart_puts(buffer);
    sprintf(buffer, "OFF is %p\n", off);
    uart_puts(buffer);

    sprintf(buffer, "MAX SECONDS is %.5f\n", MAX_SECONDS);
    uart_puts(buffer);

    reset_all_countdowns();
    sprintf(buffer, "Setting countdowns returned %d\n",
            prepare_countdowns(10, seconds, functions, in_isr));
    uart_puts(buffer);
    blue();
    run_countdown();
	while(1) {
        current = get_current_callback();
        if (current != NULL) {
            sprintf(buffer, "Running callback %p, should be %p\n", current, functions[idx++]);
            uart_puts(buffer);
            (*current)();
        }
	};
}
