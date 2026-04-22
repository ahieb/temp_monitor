#ifndef HAL_ADC_H
#define HAL_ADC_H

#include <stdint.h>

uint8_t adc_init(void);
uint16_t adc_read_temp(void);

#endif /* HAL_ADC_H */