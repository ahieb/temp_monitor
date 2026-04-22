#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "monitor_temperature.h"
#include "hal_adc.h"
#include "hal_eeprom.h"
#include "hal_led.h"
#include "hal_timer.h"

#define CRITICAL_LOW_ENTER    50
#define CRITICAL_LOW_EXIT     70
#define WARNING_HIGH_ENTER   850
#define WARNING_HIGH_EXIT    830
#define CRITICAL_HIGH_ENTER 1050
#define CRITICAL_HIGH_EXIT  1030
#define REV_A 0
#define REV_B 1
#define VREF_MV 3300    // Assume a 3.3V reference voltage for ADC
#define VREF_OFFSET_MV 500 // Offset for temperature sensor
#define ADC_FULL_SCALE 4096 // Assuming a 12-bit ADC

static int32_t temp_raw_to_temp_tenths(uint16_t temp_raw) {
    uint8_t adc_scale = eeprom_read_hw_revision() == REV_A ? 10 : 1;
    int32_t voltage_mv = ((int32_t)temp_raw * VREF_MV) / ADC_FULL_SCALE;
    int32_t temperature_tenths = (voltage_mv - VREF_OFFSET_MV) * adc_scale;
    return temperature_tenths;
}

uint8_t init_system(void) {
    char hw_serialnumber[32];

    if(adc_init()) {
        printf("ADC initialization failed\n");
        return 1;
    }

    if (eeprom_init()) {
        printf("EEPROM initialization failed\n");
        return 1;
    }

    if (led_gpios_init()) {
        printf("LED GPIO initialization failed\n");
        return 1;
    }

    eeprom_read_hw_serialnumber(hw_serialnumber, sizeof(hw_serialnumber));
    printf("Hardware Serial Number: %s\n", hw_serialnumber);

    
    if (timer_init()) {
        printf("Timer initialization failed\n");
        return 1;
    }
    return 0;
}

static led_state_t get_initial_led_state(int32_t temp)
{
    if (temp < CRITICAL_LOW_ENTER || temp >= CRITICAL_HIGH_ENTER)
        return LED_STATE_RED;

    if (temp >= WARNING_HIGH_ENTER)
        return LED_STATE_YELLOW;

    return LED_STATE_GREEN;
}

static led_state_t update_led_fsm(led_state_t led_state, int32_t temp) {
    switch (led_state) {
    case LED_STATE_GREEN:
        printf ("debug: state=GREEN, temp=%d\n", temp);
        if (temp < CRITICAL_LOW_ENTER || temp >= CRITICAL_HIGH_ENTER)
            led_state = LED_STATE_RED;
        else if (temp >= WARNING_HIGH_ENTER)
            led_state = LED_STATE_YELLOW;
        break;

    case LED_STATE_YELLOW:
        printf ("debug: state=YELLOW, temp=%d\n", temp);
        if (temp < CRITICAL_LOW_ENTER || temp >= CRITICAL_HIGH_ENTER)
            led_state = LED_STATE_RED;
        else if (temp < WARNING_HIGH_EXIT)
            led_state = LED_STATE_GREEN;
        break;

    case LED_STATE_RED:
        printf ("debug: state=RED, temp=%d\n", temp);
        if (temp >= CRITICAL_LOW_EXIT && temp < WARNING_HIGH_ENTER)
            led_state = LED_STATE_GREEN;
        else if (temp >= WARNING_HIGH_ENTER && temp < CRITICAL_HIGH_EXIT)
            led_state = LED_STATE_YELLOW;
        break;

    default:
        printf ("debug: default state=RED, temp=%d\n", temp);
        led_state = LED_STATE_RED;
        break;
    }

    return led_state;
}

void monitor_temperature(void) {
    uint16_t temp_raw = 0;
    int32_t temperature_tenths = 0;
    led_state_t led_state = LED_STATE_RED;

    temp_raw = adc_read_temp();
    temperature_tenths = temp_raw_to_temp_tenths(temp_raw);
    led_state = get_initial_led_state(temperature_tenths);
    led_set_state(led_state);

    while (1) {
        sleep(1);

        temp_raw = adc_read_temp();
        temperature_tenths = temp_raw_to_temp_tenths(temp_raw);
        led_state = update_led_fsm(led_state, temperature_tenths);
        led_set_state(led_state);
    }
}