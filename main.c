#include <avr/io.h>
#include <stdlib.h>

#include "globals.h"
#include "modes.h"

#include "AVRClock/customtimer.h"

#include <util/delay.h>

int main(void) {
	boot();
	_delay_ms(1000);
	while(1) {
        off();
	};
}
