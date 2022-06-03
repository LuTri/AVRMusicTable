#include "fncs/functionalities.h"
#include "state.h"


LOOP_PTR loop_fnc = 0;


void sound(void) {
}


void byte(uint16_t value) {
	uint8_t idx;
	for (idx = 0; idx < 16; idx++) {
		leds[idx].r = leds[idx].g = leds[idx].b = 255 * (1 & (value >> idx));
	}
	ws2812_setleds();
}

void byte_offset(uint8_t value, uint8_t offset) {
	uint8_t idx;
	if (offset + 8 < N_PACKS) {
		for (idx = 0; idx < 8; idx++) {
			leds[idx + offset].r = leds[idx + offset].g = leds[idx + offset].b = 255 * (1 & (value >> idx));
		}
		ws2812_setleds();
	}
}

void boot(void) {
	uint8_t idx;
	for (idx = 0; idx < N_PACKS; idx++) {
		leds[idx].r = 255 * (1 & (idx));
		leds[idx].g = 255 * (1 & (idx >> 1));
		leds[idx].b = 255 * (1 & (idx >> 2));
	}
	ws2812_setleds();
}

void indicate(uint8_t pos, uint8_t r, uint8_t g, uint8_t b) {
    leds[pos].r = r;
    leds[pos].g = g;
    leds[pos].b = b;
    ws2812_setleds();
}

void indicate_t(uint8_t pos, uint8_t r, uint8_t g, uint8_t b) {
    leds[pos].r = r;
    leds[pos].g = g;
    leds[pos].b = b;
}

uint8_t _add(uint8_t data) {
    return data + 1;
}

uint8_t _sub(uint8_t data) {
    return data - 1;
}

void display_byte(uint8_t row, uint8_t byte, uint8_t do_set_leds) {
    uint8_t (*mode)(uint8_t data) = 0;
    uint8_t led_pos = (row + (row % 2)) * N_COLS;
    uint8_t idx;
    uint8_t shift;

    if (row % 2) {
        mode = &_sub;
    } else {
        mode = &_add;
    }

    led_pos -= row % 2;

    for (idx = 0; idx < 8; idx++) {
        shift = 7 - idx;
        leds[led_pos].r = (1 - ((byte & (1 << shift)) >> shift)) * 30;
        leds[led_pos].g = ((byte & (1 << shift)) >> shift) * 30;
        leds[led_pos].b = 0;

        led_pos = (*mode)(led_pos);
    }

    leds[led_pos].r = 0;
    leds[led_pos].g = 0;
    leds[led_pos].b = 30;

    if (do_set_leds) {
        ws2812_setleds();
    }
}

// TODO: retreive currrent running command

void handle_data(void) {
    static uint16_t error_counter = 0;
    static uint8_t wdt_started = 0;

    if (error_counter >= UART_ERRORS_REBOOT_THRESHOLD) {
        if (!(wdt_started)) {
            /* When a total of UART_ERRORS_REBOOT_THRESHOLD errors is
             * were encountered, we try to rescue things by resetting
             * the uC completely. */
            wdt_started = 1;
            RED()
            reboot();
        }
        return;
    }
    COMMAND_BUFFER* next_command = get_next_command(&error_counter);
    if (!(next_command)) {
        return;
    }

    switch (next_command->cmd) {
        case CMD_SOUNDTOLIGHT:
            stl(next_command);
            break;
        case CMD_SLAVE:
            slave(next_command);
            break;
        case CMD_SNAKE:
            snake(next_command);
            break;
        case CMD_REBOOT:
            reboot();
            break;
        case CMD_OFF:
            OFF()
            break;
        case CMD_WHITE:
            WHITE()
            break;
        case CMD_MOOD:
        	launch_mood(next_command);
        	break;
        case CMD_WRITE:
            text_writer(next_command);
        	break;
        case CMD_SET_STATE:
            set_state(next_command);
            break;
        case CMD_GET_STATE:
            get_state();
            break;
        default:
            break;
    }
}