#include "fncs/functionalities.h"
#include "state.h"


LOOP_PTR loop_fnc = 0;


void sound(void) {
}


void byte(uint16_t value) {
	uint8_t idx;
	for (idx = 0; idx < 16; idx++) {
        uint8_t state = 255 * (1 & (value >> idx));
        set_led(idx, state, state, state);
	}
	ws2812_setleds();
}

void byte_offset(uint8_t value, uint8_t offset) {
	uint8_t idx;
	if (offset + 8 < N_PACKS) {
		for (idx = 0; idx < 8; idx++) {
            uint8_t state = 255 * (1 & (value >> idx));
            set_led(idx + offset, state, state, state);
		}
		ws2812_setleds();
	}
}

void boot(void) {
	uint8_t idx;
	for (idx = 0; idx < N_PACKS; idx++) {
        set_led(idx,
                255 * (1 & (idx)),
                255 * (1 & (idx >> 1)),
                255 * (1 & (idx >> 2)));
	}
	ws2812_setleds();
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
        set_led(led_pos,
                (1 - ((byte & (1 << shift)) >> shift)) * 30,
                ((byte & (1 << shift)) >> shift) * 30,
                0);

        led_pos = (*mode)(led_pos);
    }

    set_led(led_pos, 0, 0, 30);

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