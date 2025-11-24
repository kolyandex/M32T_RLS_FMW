#include "eeprom_data.h"
#include "flash.h"
#include "string.h"

#define EEP_CALIBS_MARKER 0x00CA11B5

static s_eeprom_data ep_data;
static s_eeprom_data ep_data_cmp;

static uint16_t crc16(const uint8_t *p, uint32_t len)
{
    uint16_t crc = 0xFFFF;
    uint8_t i;
    while (len--)
    {
        crc ^= *p++ << 8;
        for (i = 0; i < 8; i++)
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
    }
    return crc;
}

static void eeprom_data_store(void)
{
    ep_data.marker = EEP_CALIBS_MARKER;
    ep_data.struct_size = sizeof(ep_data) - sizeof(ep_data.crc);
    ep_data.crc = crc16((const uint8_t *)&ep_data, ep_data.struct_size);
    memcpy(&ep_data_cmp, &ep_data, sizeof(ep_data));
    eeprom_program(CALIB_EEPROM_ADDRESS, (unsigned char *)&ep_data, sizeof(ep_data));
}

static void eeprom_data_reset_and_save(void)
{
    ep_data.mlx_calibs.dac_level_ir_ch_a = 170;
    ep_data.mlx_calibs.dac_level_ir_ch_b = 170;
    ep_data.autolight_calibs.fast_on_thresold = 16500;
    ep_data.autolight_calibs.fast_off_thresold = 21000;
    ep_data.autolight_calibs.slow_on_thresold = 18500;
    ep_data.autolight_calibs.slow_off_thresold = 20000;
    eeprom_data_store();
}

void eeprom_data_save(void)
{
    if (memcmp(&ep_data, &ep_data_cmp, sizeof(ep_data)) != 0)
    {
        eeprom_data_store();
    }
}

void eeprom_data_init(void)
{
    if (sizeof(ep_data) & 1)
    {
        while (1)
            ;
    }
    eeprom_read(CALIB_EEPROM_ADDRESS, (unsigned char *)&ep_data, sizeof(ep_data));
    memcpy(&ep_data_cmp, &ep_data, sizeof(ep_data));
    if ((ep_data.marker != EEP_CALIBS_MARKER) ||
        (ep_data.struct_size > (sizeof(ep_data) - sizeof(ep_data.crc))) ||
        ep_data.crc != crc16((const uint8_t *)&ep_data, ep_data.struct_size))
    {
        eeprom_data_reset_and_save();
    }
}

s_mlx_adc_calibs *mlx_adc_calibs(void)
{
    return &ep_data.mlx_calibs;
}
s_autologht_calibs *autolight_calibs(void)
{
    return &ep_data.autolight_calibs;
}