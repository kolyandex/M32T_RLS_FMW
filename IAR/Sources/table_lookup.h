#pragma once
#include "derivative.h"
typedef enum
{
    TABLE_TYPE_2D = 2,
    TABLE_TYPE_3D = 3,
    TABLE_TYPE_MAX = UINT16_MAX
}e_table_type;

__packed typedef struct
{
    e_table_type type;
    uint16_t sc_x_count;
    uint16_t sc_y_count;
    uint16_t const * sc_x;
    uint16_t const * sc_y;
    uint16_t const * table;
}s_table_3d_uint16_uint16_uint16;

__packed typedef struct
{
    e_table_type type;
    uint16_t sc_x_count;
    uint16_t sc_y_count;
    uint16_t const * sc_x;
    uint8_t const * sc_y;
    uint16_t const * table;
}s_table_3d_uint16_uint8_uint16;

__packed typedef struct
{
    e_table_type type;
    uint16_t sc_count;
    uint16_t const * sc;
    uint16_t const * table;
}s_table_2d_uint16_uint16;

__packed typedef struct
{
    e_table_type type;
    uint16_t sc_count;
    uint16_t const * sc;
    uint32_t const * table;
}s_table_2d_uint16_uint32;

__packed typedef struct
{
    e_table_type type;
    uint16_t sc_count;
    uint16_t const * sc;
    uint8_t const * table;
}s_table_2d_uint16_uint8;

__packed typedef struct
{
    e_table_type type;
    uint16_t sc_count;
    uint8_t const * sc;
    uint16_t const * table;
}s_table_2d_uint8_uint16;

__packed typedef struct
{
    e_table_type type;
    uint16_t sc_count;
    int8_t const * sc;
    uint16_t const * table;
}s_table_2d_int8_uint16;

__packed typedef struct
{
    e_table_type type;
    uint16_t sc_count;
    int16_t const * sc;
    uint16_t const * table;
}s_table_2d_int16_uint16;

__packed typedef struct
{
    e_table_type type;
    uint16_t sc_count;
    uint8_t const * sc;
    uint8_t const * table;
}s_table_2d_uint8_uint8;


uint16_t LookupTable3d_uint16_uint16_uint16(uint16_t val_x, uint16_t val_y, s_table_3d_uint16_uint16_uint16 const *table);
uint16_t LookupTable3d_uint16_uint8_uint16(uint16_t val_x, uint8_t val_y, s_table_3d_uint16_uint8_uint16 const *table);
uint32_t LookupTable2d_uint16_uint32(uint16_t val, s_table_2d_uint16_uint32 const *table);
uint16_t LookupTable2d_uint16_uint16(uint16_t val, s_table_2d_uint16_uint16 const *table);
uint16_t LookupTable2d_uint16_uint8(uint16_t val, s_table_2d_uint16_uint8 const *table);
uint16_t LookupTable2d_uint8_uint16(uint8_t val, s_table_2d_uint8_uint16 const *table);
uint16_t LookupTable2d_int8_uint16(int8_t val, s_table_2d_int8_uint16 const *table);
uint16_t LookupTable2d_int16_uint16(int16_t val, s_table_2d_int16_uint16 const *table);
uint16_t LookupTable2d_uint8_uint8(uint8_t val, s_table_2d_uint8_uint8 const *table);