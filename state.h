#ifndef _STATE_H
#define _STATE_H

#include <avr/io.h>

#define STATE_DO_BENCHMARKS 0
#define STATE_DO_RANDOM     1
#define STATE_DO_SLEEP      2
#define STATE_DO_LISTEN     3
#define STATE_ERROR_REBOOT  4

#define STATE_INITIALIZED   2
#define STATE_RECOVERED     1
#define STATE_UNLOADED      0

typedef struct {
    uint16_t flags;                 // *STATE + 0
    uint16_t n_errors_reboot;       // *STATE + 2
    uint8_t benchmark_samples;      // *STATE + 4
    uint8_t reboot_time_error;      // *STATE + 5
    uint8_t reboot_time_general;    // *STATE + 6
    uint8_t current_mode;           // *STATE + 7
    uint16_t checksum;              // *STATE + 8
} STATE;

uint8_t recover_state(void);
void initialize_state(void);
void archive_state(void);

void archive_state_member(uint8_t* member_addr);
STATE* _get_state_ptr(void);

#define GET_STATE(member) \
    (_get_state_ptr()->member)

#define GET_STATE_FLAG(flag) \
    (GET_STATE(flags) & (1 << flag))

#define PUT_STATE(member, value) \
    (_get_state_ptr()->member = value; archive_state_member(&(_get_state_ptr()->member)));

extern STATE state;

#endif