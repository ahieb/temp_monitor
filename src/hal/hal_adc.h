#ifndef HAL_ADC_H
#define HAL_ADC_H

#include <stdint.h>
#include <stdbool.h>

extern volatile bool g_adc_ready;
uint8_t adc_init(void (*adc_ready_callback)(void));
uint16_t adc_read_temp(void);
void adc_start_sampling(void);

#endif /* HAL_ADC_H */