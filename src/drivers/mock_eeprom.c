#include <stdio.h>
#include "hal_eeprom.h"

uint8_t eeprom_init(void) {
    printf("[EEPROM] Initialized\n");
    return 0;
}

uint8_t eeprom_read_hw_revision(void) {
    return 1; // Mock hardware revision
}

uint8_t eeprom_read_hw_serialnumber(char *buf, uint8_t buf_len) {
    uint8_t i = 0;
    const char mock_serial[] = "SN-12345678";
    if (buf_len < sizeof(mock_serial)) return 1;
    for (i = 0; i < sizeof(mock_serial); i++) {
        buf[i] = mock_serial[i];
    }
    return 0;
}