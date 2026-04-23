#ifndef HAL_LED_H
#define HAL_LED_H

#include <stdint.h>

#define GPIO_LEDS_OFF 0
#define GPIO_LED_RED 1
#define GPIO_LED_YELLOW 2
#define GPIO_LED_GREEN 3

typedef enum {
    LED_STATE_GREEN,
    LED_STATE_YELLOW,
    LED_STATE_RED
} led_state_t;

uint8_t led_gpios_init(void);
uint8_t led_set_state(led_state_t state);

#endif /* HAL_LED_H */