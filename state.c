#include "state.h"
#include "OdroidUart/basic/fletcher.h"
#include "OdroidUart/avr-uart/uart.h"
#include "OdroidUart/pyconversion.h"

#include <avr/eeprom.h>

uint8_t eeState[sizeof(STATE)] EEMEM;

STATE state = { 0 };

STATE* CONSITENT_STATE = &state;
uint8_t _loaded = STATE_UNLOADED;

inline uint16_t create_state_checksum(void) {
    return fletchers_checksum((uint8_t*)&state, sizeof(STATE) - sizeof(uint16_t));
}

inline uint8_t _verify_state(void) {
    return create_state_checksum() == state.checksum;
}

inline void _update_checksum(void) {
    state.checksum = create_state_checksum();
}

void initialize_state(void) {
    state.checksum = 0;
    state.flags = (1 << STATE_ERROR_REBOOT) | (1 << STATE_DO_LISTEN);
    state.benchmark_samples = 0;
    state.n_errors_reboot = 0;
    state.reboot_time_error = 0;
    state.reboot_time_general = 0;
    state.current_mode = 0;
    for (uint8_t idx = 0; idx < N_ROWS; idx++) {
        state.stl_hues[idx] = 0;
    }
    state.stl_intensity = 0;
    state.stl_fnc_counts = 0;
    state.stl_dim_counts = 0;
    state.stl_errors = 0;
}

void archive_state(void) {
    state.checksum = create_state_checksum();
    eeprom_update_block((uint8_t*)&state, eeState, sizeof(STATE));
}

uint8_t recover_state(void) {
    eeprom_read_block((uint8_t*)&state, eeState, sizeof(STATE));
    if (!(_verify_state())) {
        initialize_state();
        return STATE_INITIALIZED;
    }
    return STATE_RECOVERED;
}

void startup_state(void) {
    if (_loaded == STATE_UNLOADED) _loaded = recover_state();
}

STATE* get_state_ptr(void) {
    startup_state();
    return &state;
}

/* PUBLIC API */

void set_state(COMMAND_BUFFER* command) {
    startup_state();
    float* hue_ptr = (float*)(&(command->data[16]));

    state.flags = dualbyte(command->data[0], command->data[1]);
    state.n_errors_reboot = dualbyte(command->data[2], command->data[3]);
    state.benchmark_samples = command->data[4];
    state.reboot_time_error = command->data[5];
    state.reboot_time_general = command->data[6];
    state.current_mode = command->data[7];
    state.stl_errors = dualbyte(command->data[8], command->data[9]);

    state.stl_intensity = dualbyte(
        command->data[11],
        command->data[10]
    );
    state.stl_fnc_counts = dualbyte(
        command->data[12],
        command->data[13]
    );
    state.stl_dim_counts = dualbyte(
        command->data[14],
        command->data[15]
    );

    for (uint8_t idx = 0; idx < N_ROWS; idx++) {
        state.stl_hues[idx] = hue_ptr[idx];
    }

    archive_state();
}

void get_state(void) {
    startup_state();
    uart0_puts(MSG_STATE_DATA);

    uart0_putc(_loaded);

    uart0_putc(((uint8_t*)&state.flags)[1]);
    uart0_putc(((uint8_t*)&state.flags)[0]);
    
    uart0_putc(((uint8_t*)&state.n_errors_reboot)[1]);
    uart0_putc(((uint8_t*)&state.n_errors_reboot)[0]);

    uart0_putc(((uint8_t*)&state.benchmark_samples)[0]);
    uart0_putc(((uint8_t*)&state.reboot_time_error)[0]);
    uart0_putc(((uint8_t*)&state.reboot_time_general)[0]);
    uart0_putc(((uint8_t*)&state.current_mode)[0]);

    uart0_putc(((uint8_t*)&state.stl_errors)[1]);
    uart0_putc(((uint8_t*)&state.stl_errors)[0]);

    uart0_putc(((uint8_t*)&state.stl_intensity)[0]);
    uart0_putc(((uint8_t*)&state.stl_intensity)[1]);

    uart0_putc(((uint8_t*)&state.stl_fnc_counts)[1]);
    uart0_putc(((uint8_t*)&state.stl_fnc_counts)[0]);

    uart0_putc(((uint8_t*)&state.stl_dim_counts)[1]);
    uart0_putc(((uint8_t*)&state.stl_dim_counts)[0]);

    for (uint8_t idx = 0; idx < sizeof(state.stl_hues); idx++) {
        uart0_putc(((uint8_t*)&state.stl_hues)[idx]);
    }
    
    uart0_putc(((uint8_t*)&state.checksum)[1]);
    uart0_putc(((uint8_t*)&state.checksum)[0]);

    uart0_puts(MSG_STATE_DATA_STOP);
}
