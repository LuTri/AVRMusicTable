#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "globals.h"
#include "modes.h"

#include "AVRClock/customtimer.h"

#include <util/delay.h>

uint8_t mcusr __attribute__ ((section (".noinit")));

void echo_uart(void) {
    char buf[100];
    char echo[130];
    uart_gets(buf, 100);

    sprintf(echo, "!ECHO! %s !ECHO!\n", buf);
    uart_puts(echo);
}

void resetFlagsInit(void) __attribute__ ((naked))
                          __attribute__ ((used))
                          __attribute__ ((section (".init0")));
void resetFlagsInit(void)
{
  /*
   * save the reset flags passed from the bootloader
   * This is a "simple" matter of storing (STS) r2 in the special variable
   * that we have created.  We use assembler to access the right variable.
   */
  __asm__ __volatile__ ("sts %0, r2\n" : "=m" (mcusr) :);
}

int main(void) {
	while(1) {
        if (has_incoming()) {
            slave();
        }
    }
}
