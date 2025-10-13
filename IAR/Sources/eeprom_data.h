#pragma once
#include "derivative.h"

__packed typedef struct
{
    uint8_t dac_level_ir_ch_a;
    uint8_t dac_level_ir_ch_b;
} s_mlx_adc_calibs;

__packed typedef struct
{
    uint16_t fast_on_thresold;
    uint16_t fast_off_thresold;
    uint16_t slow_on_thresold;
    uint16_t slow_off_thresold;
} s_autologht_calibs;

__packed typedef struct
{
    uint32_t marker;
    uint16_t struct_size;
    s_mlx_adc_calibs mlx_calibs;
    s_autologht_calibs autolight_calibs;
    uint16_t crc;
} s_eeprom_data;

void eeprom_data_init(void);
void eeprom_data_save(void);

s_mlx_adc_calibs *mlx_adc_calibs(void);
s_autologht_calibs *autolight_calibs(void);