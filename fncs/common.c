#include "functionalities.h"
#include <avr/wdt.h>
#include <avr/boot.h>

uint8_t* loop_data = 0;
uint8_t DO_BENCHMARK = 0; // REPLACE ME WITH STATE

void reboot(void) {
    wdt_enable(WDTO_4S);
    loop_fnc = 0;
}

void rgb(uint8_t r, uint8_t g, uint8_t b) {
	uint8_t idx;
	for (idx = 0; idx < N_PACKS; idx++) set_led(idx, r, g, b);
	loop_fnc = 0;
	ws2812_setleds();
}

void get_fuses(uint8_t* low, uint8_t* high, uint8_t* extended) {
    *high = boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS);
    *low = boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS);
    *extended = boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS);
}