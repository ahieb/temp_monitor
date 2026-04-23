#ifndef HAL_EEPROM_H
#define HAL_EEPROM_H

#include <stdint.h>

#define EEPROM_SIZE 1024 //assuming 1KB of EEPROM

uint8_t eeprom_init(void);
uint8_t eeprom_read_hw_revision(void);
uint8_t eeprom_read_hw_serialnumber(char *, uint8_t);

#endif /* HAL_EEPROM_H */