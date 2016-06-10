#include <avr/io.h>

#include "globals.h"
#include "modes.h"

#include "AVRClock/customtimer.h"

extern uint8_t mode;

void _cycle(void) {
	mode = CMD_MOOD;
}

int main(void) {
	float seconds[1] = {3.0};
	uart_init();

	CustomTimer* timer = get_timer(0);

	prepare_countdown(timer, seconds, 1, &_cycle);

	while(1) {
		if (mode == CMD_SLAVEMODE) {
			slave();
		} else if (mode == CMD_MOOD) {
			mood();
		} else if (mode == CMD_SOUNDTOLIGHT) {
			sound();
		} else if (mode == CMD_WHITE) {
			white();
		} else {
			mode = CMD_SLAVEMODE;
		}
	}
}
