#include <avr/io.h>

#include "globals.h"
#include "modes.h"

#include "AVRClock/customtimer.h"

#include <util/delay.h>

int main(void) {
//	uart_init();
	float seconds[10];
	T_CALLBACK fncs[10];

	seconds[0] = seconds[2] = seconds[4] = seconds[6] = seconds[8] = 1.5;
	seconds[1] = seconds[3] = seconds[5] = seconds[7] = seconds[9] = 10;

	fncs[0] = fncs[2] = fncs[4] = fncs[6] = fncs[8] = &off;
	fncs[1] = fncs[3] = fncs[5] = fncs[7] = fncs[9] = &white;
	
	boot();
	_delay_ms(1000);
	off();

	prepare_countdown(10, seconds, fncs);
	run_countdown();

//	while(1) {
///		if (mode == CMD_SLAVEMODE) {
///			slave();
///		} else if (mode == CMD_MOOD) {
///			mood();
///		} else if (mode == CMD_SOUNDTOLIGHT) {
///			sound();
///		} else if (mode == CMD_WHITE) {
///			white();
///		} else if (mode == CMD_OFF) {
///			off();
///		} else {
///			mode = CMD_SLAVEMODE;
///		}
/////	}
	while(1) {
		if (0) {
			mood();
		}
	};
}
