#ifndef _STATE_H
#define _STATE_H

#include <avr/io.h>
#include "OdroidUart/commands.h"

#define STATE_DO_BENCHMARKS 0
#define STATE_DO_RANDOM     1
#define STATE_DO_SLEEP      2
#define STATE_DO_LISTEN     3
#define STATE_ERROR_REBOOT  4

#define STATE_INITIALIZED   2
#define STATE_RECOVERED     1
#define STATE_UNLOADED      0

typedef struct {
    uint16_t flags;
    uint16_t n_errors_reboot;
    uint8_t benchmark_samples;
    uint8_t reboot_time_error;
    uint8_t reboot_time_general;
    uint8_t current_mode;
    uint16_t stl_errors;
    uint16_t stl_intensity;
    uint16_t stl_fnc_counts;
    uint16_t stl_dim_counts;
    float stl_hues[N_ROWS];
    uint16_t checksum;
} STATE;

void archive_state_member(uint8_t* member_addr);
STATE* get_state_ptr(void);

#define UPDATE_MEMBER(member, write_checksum) \
    (update_eeprom(&member, sizeof(member), write_checksum))

#define GET_STATE(member) \
    (get_state_ptr()->member)

#define GET_STATE_FLAG(flag) \
    (GET_STATE(flags) & (1 << flag))

#define PUT_STATE(member, value) \
    (get_state_ptr()->member = value; UPDATE_MEMBER(get_state_ptr()->member, 1));


extern STATE state;

void set_state(COMMAND_BUFFER* command);
void get_state(void);

#endif
