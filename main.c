#include <avr/io.h>

#include "config.h"
#include "modes.h"

extern uint8_t mode;

int main(void) {
	uart_init();

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
