#include "hal_led.h"
#include <stdio.h>
#include "debug.h"

uint8_t led_gpios_init(void) {
    DEBUG_PRINTF("[LED] GPIOs initialized with LEDs off\n");
    return 0;
}

uint8_t led_set_state(led_state_t state) {
    switch (state) {
    case LED_STATE_GREEN:
        DEBUG_PRINTF("[LED] GREEN\n");
        break;
    case LED_STATE_YELLOW:
        DEBUG_PRINTF("[LED] YELLOW\n");
        break;
    case LED_STATE_RED:
        DEBUG_PRINTF("[LED] RED\n");
        break;
    default:
        DEBUG_PRINTF("[LED] ERROR: invalid state %d\n", state);
        return 1;
    }
    return 0;
}
