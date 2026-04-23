#include "hal_timer.h"
#include <stdio.h>
#include <unistd.h>
#include "debug.h"

static void (*g_timer_callback)(void) = NULL;

uint8_t timer_init(uint32_t period_us, void (*timer_callback)(void)) {
    g_timer_callback = timer_callback;
    DEBUG_PRINTF("[TIMER] Initialized with period %dus\n", period_us);
    return 0;
}

uint8_t timer_start(void) {
    DEBUG_PRINTF("[TIMER] Timer started\n");
    sleep(1); // Simulate timer delay
    g_timer_callback(); // Call the timer callback
    return 0;
}
