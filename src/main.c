
#include <stdbool.h>
#include <stdio.h>
#include "monitor_temperature.h"
#include "debug.h"

int main(void) {
    if (init_system()) {
        DEBUG_PRINTF("Error initializing system.\n");
        return 1;
    }

    monitor_temperature();

    return 0;
}