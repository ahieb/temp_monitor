#include "hal_adc.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "debug.h"

static void (*g_adc_ready_callback)(void) = NULL;

uint8_t adc_init(void (*adc_ready_callback)(void)) {
    g_adc_ready_callback = adc_ready_callback;
    DEBUG_PRINTF("[ADC] Initialized\n");
    return 0;
}

uint16_t adc_read_temp(void) {
    uint16_t mock_temp_raw = (rand() % (807 - 596 + 1)) + 596; // limit to -20°C to 150°C
    DEBUG_PRINTF("[ADC]: Raw value is read out as %d\n", mock_temp_raw);
    return mock_temp_raw;
}

void adc_start_sampling(void) {
    DEBUG_PRINTF("[ADC]: Sampling started, will trigger callback when ready\n");
    sleep (1); // Simulate sampling delay
    g_adc_ready_callback(); // mock real callback
}
