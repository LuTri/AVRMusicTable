#include "state.h"
#include "OdroidUart/basic/fletcher.h"

#include <avr/eeprom.h>

uint8_t eeState[sizeof(STATE)] EEMEM;

STATE state = { 0 };

STATE* CONSITENT_STATE = &state;
uint8_t _loaded = STATE_UNLOADED;

inline uint16_t create_state_checksum(void) {
    return fletchers_checksum((uint8_t*)&state, sizeof(STATE) - 2);
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
}

void update_state_byte_at(uint8_t idx) {
    uint8_t size_t = 0;
    if (idx < 4) size_t = 2;
    else size_t = 1;

    eeprom_update_block(((uint8_t*)&state) + idx, eeState + idx, size_t);
    /* always update the checksum accordingly */
    _update_checksum();
    eeprom_update_block(((uint8_t*)&state) + (sizeof(STATE) - 2), eeState + (sizeof(STATE) - 2), 2);
}

void archive_state_member(uint8_t* member_addr) {
    uint8_t* _state = (uint8_t*)&state;
    uint8_t* addr_checksum = (uint8_t*)&(state.checksum);

    uint8_t ptr_inc = 0;
    while ((_state + ptr_inc) != member_addr
           && (_state + ptr_inc) != addr_checksum) ptr_inc++;

    if ((_state + ptr_inc) == addr_checksum) {
        /* INVALID?! WHAT NOW?! */
    }

    update_state_byte_at(ptr_inc);
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

STATE* _get_state_ptr(void) {
    startup_state();
    return &state;
}