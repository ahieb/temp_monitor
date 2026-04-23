#ifndef HAL_TIMER_H
#define HAL_TIMER_H

#include <stdint.h>
#include <stdbool.h>

extern volatile bool g_timer_expired;
uint8_t timer_init(uint32_t, void (*timer_callback)(void));
uint8_t timer_start(void);

#endif /* HAL_TIMER_H */