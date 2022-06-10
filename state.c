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
}

void update_state_byte_at(uint8_t idx, uint8_t write_checksum) {
    uint8_t size_t = 0;
    if (idx < 4) size_t = sizeof(uint16_t);
    else if (idx < 8) size_t = sizeof(uint8_t);
    else if (idx < 14) size_t = sizeof(uint16_t);
    else size_t = sizeof(float) * N_COLS;

    eeprom_update_block(((uint8_t*)&state) + idx, eeState + idx, size_t);

    if (write_checksum) {
        _update_checksum();
        eeprom_update_block(((uint8_t*)&state) + (sizeof(STATE) - sizeof(uint16_t)), eeState + (sizeof(STATE) - sizeof(uint16_t)), sizeof(uint16_t));
    }
}

void archive_state_member(uint8_t* member_addr) {
    uint8_t* _state = (uint8_t*)&state;
    uint8_t* addr_checksum = (uint8_t*)&(state.checksum);

    uint8_t ptr_inc = 0;
    while ((_state + ptr_inc) != member_addr
           && (_state + ptr_inc) != addr_checksum) ptr_inc++;

    update_state_byte_at(ptr_inc, 1);
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
    float* hue_ptr = (float*)(&(command->data[6]));

    state.stl_intensity = dualbyte(
        command->data[0],
        command->data[1]
    );
    state.stl_fnc_counts = dualbyte(
        command->data[2],
        command->data[3]
    );
    state.stl_dim_counts = dualbyte(
        command->data[4],
        command->data[5]
    );

    for (uint8_t idx = 0; idx < N_ROWS; idx++) {
        state.stl_hues[idx] = hue_ptr[idx];
    }
    update_state_byte_at(8, 0);
    update_state_byte_at(10, 0);
    update_state_byte_at(12, 0);
    update_state_byte_at(14, 1);
}

void get_state(void) {
    startup_state();
    uart0_puts(MSG_STATE_DATA);

    uart0_putc(_loaded);

    uart0_putc(((uint8_t*)&state.stl_intensity)[1]);
    uart0_putc(((uint8_t*)&state.stl_intensity)[0]);

    uart0_putc(((uint8_t*)&state.stl_fnc_counts)[1]);
    uart0_putc(((uint8_t*)&state.stl_fnc_counts)[0]);

    uart0_putc(((uint8_t*)&state.stl_dim_counts)[1]);
    uart0_putc(((uint8_t*)&state.stl_dim_counts)[0]);

    for (uint8_t idx = 0; idx < N_ROWS; idx++) {
        float* cur = &(state.stl_hues[idx]);
        for (uint8_t f_idx = 0; f_idx < sizeof(float); f_idx++) {
            uart0_putc(((uint8_t*)cur)[f_idx]);
        }
    }
    uart0_puts(MSG_STATE_DATA_STOP);
}
