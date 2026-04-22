#include "hal_led.h"
#include <stdio.h>

uint8_t led_gpios_init(void) {
    printf("[LED] GPIOs initialized with LEDs off\n");
    return 0;
}

uint8_t led_set_state(led_state_t state) {
    switch (state) {
    case LED_STATE_GREEN:
        printf("[LED] GREEN\n");
        break;
    case LED_STATE_YELLOW:
        printf("[LED] YELLOW\n");
        break;
    case LED_STATE_RED:
        printf("[LED] RED\n");
        break;
    default:
        printf("[LED] ERROR: invalid state %d\n", state);
        return 1;
    }
    return 0;
}
