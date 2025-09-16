#include "derivative.h"
#include "table_lookup.h"

#define CALIBRATIONS_IN_RAM 0
#define NO_CALIBRATIONS_AREA 1

#if (NO_CALIBRATIONS_AREA == 1)
#define __CONST const
#define CAL_SEC
#define CAL_SEC_TBL_DEF
#else
#if (CALIBRATIONS_IN_RAM == 1)
#define __CONST
#define CAL_SEC __attribute__((section(".calibrations_ram")))
#else
#define __CONST const
#define CAL_SEC __attribute__((section(".calibrations")))
#endif

#define CAL_SEC_TBL_DEF __attribute__((section(".calibrations")))
#endif

__root static __CONST uint16_t scale_Adc[] CAL_SEC = {0, 96, 160, 288, 500, 700, 900, 1100, 1300, 1500, 1700, 1900, 2100, 2150, 2300, 2500};
__root static __CONST uint8_t table_Adc[] CAL_SEC =  {5, 16,  30,  50,  70,  80, 100,  120,  130,  150,  170,  190,  200,  215,  230,  250};

__root const s_table_2d_uint16_uint8 TBL_ADC_Conv CAL_SEC_TBL_DEF =
    {
        .type = TABLE_TYPE_2D,
        .sc_count = SIZEOF_ARR(scale_Adc),
        .sc = scale_Adc,
        .table = table_Adc};

__root static __CONST uint16_t scale_RawAmbientLightData_0[] CAL_SEC = {9000, 12891, 13288, 17128, 18488, 19727, 20671, 21173, 21903, 22883, 23956, 25445, 27212, 29000,  30500,  32000};
__root static __CONST uint32_t table_RawAmbientLightData_0[] CAL_SEC = {   0,   110,   245,   450,   800,  2026,  4000,  5600,  8500, 11000, 19000, 33000, 45000, 70000, 100000, 127000};

__root const s_table_2d_uint16_uint32 TBL_HUD_AmbientLight CAL_SEC_TBL_DEF =
    {
        .type = TABLE_TYPE_2D,
        .sc_count = SIZEOF_ARR(scale_RawAmbientLightData_0),
        .sc = scale_RawAmbientLightData_0,
        .table = table_RawAmbientLightData_0};

__root static __CONST uint16_t scale_RawAmbientLightData_1[] CAL_SEC = {10000, 17585, 18360, 19380, 19890, 20145, 20400, 20655, 20910, 21165, 21420, 21675, 22440, 22695, 22950, 23460};
__root static __CONST uint32_t table_RawAmbientLightData_1[] CAL_SEC = {    0,    50,    85,   122,   170,   208,   233,   267,   300,   333,   417,   500,   635,   733,   933,  1021};

__root const s_table_2d_uint16_uint32 TBL_RawAmbientLightData_1 CAL_SEC_TBL_DEF =
    {
        .type = TABLE_TYPE_2D,
        .sc_count = SIZEOF_ARR(scale_RawAmbientLightData_1),
        .sc = scale_RawAmbientLightData_1,
        .table = table_RawAmbientLightData_1};

__root static __CONST uint16_t scale_RawAmbientLightData_2[] CAL_SEC = {21500, 26031, 27736, 29123, 29785, 31753, 31935, 32373, 33584, 33796, 34080, 35000, 35220, 35420, 35700, 36000};
__root static __CONST uint32_t table_RawAmbientLightData_2[] CAL_SEC = {    0,    20,    50,    70,    80,   100,   120,   130,   140,   150,   160,   180,   200,   220,   245,   250};

__root const s_table_2d_uint16_uint32 TBL_RawAmbientLightData_2 CAL_SEC_TBL_DEF =
    {
        .type = TABLE_TYPE_2D,
        .sc_count = SIZEOF_ARR(scale_RawAmbientLightData_2),
        .sc = scale_RawAmbientLightData_2,
        .table = table_RawAmbientLightData_2};

__root const s_table_2d_uint16_uint32 * TBL_RawAmbientLightData[TOTAL_AMB_LIGHT_CHANNELS] = 
{
    &TBL_HUD_AmbientLight,
    &TBL_RawAmbientLightData_1,
    &TBL_RawAmbientLightData_2
};