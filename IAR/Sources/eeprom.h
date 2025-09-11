#pragma once
#include "derivative.h"

void eeprom_init(int freq);
int eeprom_read(int addr, unsigned char * data, int len);
int eeprom_write(int addr, unsigned char * data, int len);