#include <stdio.h>
#include <stdint.h>
#include "monitor_temperature.h"
#include "hal_adc.h"
#include "hal_eeprom.h"
#include "hal_led.h"
#include "hal_timer.h"
#include "debug.h"

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
#define SAMPLE_INTERVAL_US 100

void timer_expired_callback(void);
void adc_data_ready_callback(void);

volatile bool g_timer_expired = false;
volatile bool g_adc_ready = false;


static int32_t temp_raw_to_temp_tenths(uint16_t temp_raw) {
    uint8_t adc_scale = eeprom_read_hw_revision() == REV_A ? 10 : 1;
    int32_t voltage_mv = ((int32_t)temp_raw * VREF_MV) / ADC_FULL_SCALE;
    int32_t temperature_tenths = (voltage_mv - VREF_OFFSET_MV) * adc_scale;
    DEBUG_PRINTF("debug: temp_raw=%u, voltage_mv=%d, temperature_tenths=%d\n", temp_raw, voltage_mv, temperature_tenths);
    return temperature_tenths;
}

uint8_t init_system(void) {
    char hw_serialnumber[32];

    if(adc_init(adc_data_ready_callback)) {
        DEBUG_PRINTF("ADC initialization failed\n");
        return 1;
    }
    else {
        adc_start_sampling(); // once for initial value
    }

    if (eeprom_init()) {
        DEBUG_PRINTF("EEPROM initialization failed\n");
        return 1;
    }

    if (led_gpios_init()) {
        DEBUG_PRINTF("LED GPIO initialization failed\n");
        return 1;
    }

    eeprom_read_hw_serialnumber(hw_serialnumber, sizeof(hw_serialnumber));
    DEBUG_PRINTF("Hardware Serial Number: %s\n", hw_serialnumber);

    
    if (timer_init(SAMPLE_INTERVAL_US, timer_expired_callback)) {
        DEBUG_PRINTF("Timer initialization failed\n");
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

static led_state_t update_led_fsm(int32_t temp, led_state_t led_state) {
    switch (led_state) {
    case LED_STATE_GREEN:
        DEBUG_PRINTF ("debug: state=GREEN, temp=%d\n", temp);
        if (temp < CRITICAL_LOW_ENTER || temp >= CRITICAL_HIGH_ENTER)
            led_state = LED_STATE_RED;
        else if (temp >= WARNING_HIGH_ENTER)
            led_state = LED_STATE_YELLOW;
        break;

    case LED_STATE_YELLOW:
        DEBUG_PRINTF ("debug: state=YELLOW, temp=%d\n", temp);
        if (temp < CRITICAL_LOW_ENTER || temp >= CRITICAL_HIGH_ENTER)
            led_state = LED_STATE_RED;
        else if (temp < WARNING_HIGH_EXIT)
            led_state = LED_STATE_GREEN;
        break;

    case LED_STATE_RED:
        DEBUG_PRINTF ("debug: state=RED, temp=%d\n", temp);
        if (temp >= CRITICAL_LOW_EXIT && temp < WARNING_HIGH_ENTER)
            led_state = LED_STATE_GREEN;
        else if (temp >= WARNING_HIGH_ENTER && temp < CRITICAL_HIGH_EXIT)
            led_state = LED_STATE_YELLOW;
        break;

    default:
        DEBUG_PRINTF ("debug: default state=RED, temp=%d\n", temp);
        led_state = LED_STATE_RED;
        break;
    }
    return led_state;   // new state
}

void timer_expired_callback(void) {
    g_timer_expired = true;
    return;
}

void adc_data_ready_callback(void) {
    g_adc_ready = true;
    return;
}

void monitor_temperature(void) {
    uint16_t temp_raw = adc_read_temp();
    int32_t temperature_tenths = temp_raw_to_temp_tenths(temp_raw);
    led_state_t led_state = get_initial_led_state(temperature_tenths);
    
    if(led_set_state(led_state)) {
        DEBUG_PRINTF("Failed to set initial LED state\n");
        return;
    }

    if (timer_start()) {
        DEBUG_PRINTF("Failed to start timer\n");
        return;
    }

    while (1) {
        if (g_timer_expired) {
            #ifndef DEBUG
                g_timer_expired = false;
            #endif
            adc_start_sampling();
        }
        if (g_adc_ready) {
            #ifndef DEBUG
                g_adc_ready = false;
            #endif
            uint16_t temp_raw = adc_read_temp();
            int32_t temperature_tenths = temp_raw_to_temp_tenths(temp_raw);
            led_state = update_led_fsm(temperature_tenths, led_state);
            if(led_set_state(led_state)) {
                DEBUG_PRINTF("Failed to set LED state\n");
            }
        }
    }
}