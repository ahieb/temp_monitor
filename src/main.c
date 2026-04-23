
#include <stdbool.h>
#include <stdio.h>
#include "monitor_temperature.h"

int main(void) {
    if (init_system()) {
        printf("Error initializing system.\n");
        return 1;
    }

    monitor_temperature();

    return 0;
}