#pragma once

#include <stdint.h>

/* Flash driver errors */

#define FLASH_ERR_SUCCESS                             0
#define FLASH_ERR_INVALID_PARAM                       0x001U        // invalid parameter error code
#define EEPROM_ERR_SINGLE_BIT_FAULT                   0x002U        // EEPROM single bit fault error code
#define EEPROM_ERR_DOUBLE_BIT_FAULT                   0x004U        // EEPROM double bits fault error code
#define FLASH_ERR_BUSY                                0x008U        // flash busy error code
#define FLASH_ERR_ACCESS                              0x010U        // flash access error code
#define FLASH_ERR_PROTECTION                          0x020U        // flash protection error code
#define FLASH_ERR_MGSTAT0                             0x040U        // flash verification error code
#define FLASH_ERR_MGSTAT1                             0x080U        // flash non-correctable error code
#define FLASH_ERR_INIT_CCIF                           0x100U        // flash driver init error with CCIF = 1
#define FLASH_ERR_INIT_FDIV                           0x200U        // flash driver init error with wrong FDIV

uint16_t flash_init(uint32_t freq);
uint16_t flash_erase_sector(uint32_t addr);
uint16_t eeprom_erase_sector(uint32_t addr);
uint16_t flash_write(uint32_t addr, const unsigned char *data, uint16_t len);
uint16_t flash_program(uint32_t addr, const unsigned char *data, uint16_t len);
uint16_t eeprom_program(uint32_t addr, const unsigned char *data, uint16_t len);
uint16_t eeprom_read(uint32_t addr, unsigned char *data, uint16_t len);
