#include "hal_timer.h"
#include <stdio.h>

#define DEBUG

volatile bool g_timer_expired = false;
static void (*g_timer_callback)(void) = NULL;

uint8_t timer_init(uint32_t period_us, void (*timer_callback)(void)) {
    g_timer_callback = timer_callback;
    printf("[TIMER] Initialized with period %dus\n", period_us);
    return 0;
}

uint8_t timer_start(void) {
    printf("[TIMER] Timer started\n");
    return 0;
}

void timer_isr(void) {
    g_timer_expired = true;
    #ifdef DEBUG
        printf("[TIMER] Timer expired, ISR called\n"); //only for demonstration
    #endif
}