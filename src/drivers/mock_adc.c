#include "hal_adc.h"
#include <stdio.h>

#define MOCK_TEMP_RAW    850

static uint16_t mock_temp_raw = MOCK_TEMP_RAW;

uint8_t adc_init(void) {
    printf("[ADC] Initialized\n");
    return 0;
}

uint16_t adc_read_temp(void) {
    printf("[ADC]: Temperature is read out\n");
    return mock_temp_raw;
}
