#include "hal_adc.h"
#include <stdio.h>

#define MOCK_TEMP_RAW    850

volatile bool g_adc_ready = false;
static uint16_t mock_temp_raw = MOCK_TEMP_RAW;
static void (*g_adc_ready_callback)(void) = NULL;

uint8_t adc_init(void (*adc_ready_callback)(void)) {
    g_adc_ready_callback = adc_ready_callback;
    printf("[ADC] Initialized\n");
    return 0;
}

uint16_t adc_read_temp(void) {
    printf("[ADC]: Temperature is read out\n");
    return mock_temp_raw;
}

void adc_start_sampling(void) {
    printf("[ADC]: Sampling started, will trigger isr when ready\n");
}

void adc_ready_isr(void) {
    g_adc_ready = true;
}