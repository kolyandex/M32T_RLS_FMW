#include "table_lookup.h"

typedef struct
{
    uint16_t idx1;
    uint16_t idx2;
} s_scale_idx;

static s_scale_idx LookupScale_uint16(uint16_t val, const uint16_t *sc, uint32_t n)
{
    s_scale_idx ret = {.idx1 = 0, .idx2 = 0};
    if ((val <= sc[0]) || (n < 2))
    {
    }
    else if (val >= sc[n - 1])
    {
        ret.idx1 = n - 1;
        ret.idx2 = n - 1;
    }
    else
    {
        for (int i = 0; i < n - 1; i++)
        {
            if (val == sc[i])
            {
                ret.idx1 = i;
                ret.idx2 = i;
                break;
            }
            if ((val > sc[i]) && (val < sc[i + 1]))
            {
                ret.idx1 = i;
                ret.idx2 = i + 1;
                break;
            }
        }
    }
    return ret;
}

static s_scale_idx LookupScale_uint8(uint8_t val, const uint8_t *sc, uint32_t n)
{
    s_scale_idx ret = {.idx1 = 0, .idx2 = 0};
    if ((val <= sc[0]) || (n < 2))
    {
    }
    else if (val >= sc[n - 1])
    {
        ret.idx1 = n - 1;
        ret.idx2 = n - 1;
    }
    else
    {
        for (int i = 0; i < n - 1; i++)
        {
            if (val == sc[i])
            {
                ret.idx1 = i;
                ret.idx2 = i;
                break;
            }
            if ((val > sc[i]) && (val < sc[i + 1]))
            {
                ret.idx1 = i;
                ret.idx2 = i + 1;
                break;
            }
        }
    }
    return ret;
}
static s_scale_idx LookupScale_int8(int8_t val, const int8_t *sc, uint32_t n)
{
    s_scale_idx ret = {.idx1 = 0, .idx2 = 0};
    if ((val <= sc[0]) || (n < 2))
    {
    }
    else if (val >= sc[n - 1])
    {
        ret.idx1 = n - 1;
        ret.idx2 = n - 1;
    }
    else
    {
        for (int i = 0; i < n - 1; i++)
        {
            if (val == sc[i])
            {
                ret.idx1 = i;
                ret.idx2 = i;
                break;
            }
            if ((val > sc[i]) && (val < sc[i + 1]))
            {
                ret.idx1 = i;
                ret.idx2 = i + 1;
                break;
            }
        }
    }
    return ret;
}
static s_scale_idx LookupScale_int16(int16_t val, const int16_t *sc, uint32_t n)
{
    s_scale_idx ret = {.idx1 = 0, .idx2 = 0};
    if ((val <= sc[0]) || (n < 2))
    {
    }
    else if (val >= sc[n - 1])
    {
        ret.idx1 = n - 1;
        ret.idx2 = n - 1;
    }
    else
    {
        for (int i = 0; i < n - 1; i++)
        {
            if (val == sc[i])
            {
                ret.idx1 = i;
                ret.idx2 = i;
                break;
            }
            if ((val > sc[i]) && (val < sc[i + 1]))
            {
                ret.idx1 = i;
                ret.idx2 = i + 1;
                break;
            }
        }
    }
    return ret;
}

uint16_t LookupTable2d_uint8_uint16(uint8_t val, s_table_2d_uint8_uint16 const *table)
{
    s_scale_idx x = LookupScale_uint8(val, table->sc, table->sc_count);
    int x_1_1 = table->table[x.idx1];
    int x_1_2 = table->table[x.idx2];
    int percent_x_x100 = (x.idx1 == x.idx2) ? 0 : ((val - table->sc[x.idx1]) * 100) / (table->sc[x.idx2] - table->sc[x.idx1]);
    uint16_t ret = 0;
    if (x_1_2 < x_1_1)
    {
        ret = x_1_1 - ((x_1_1 - x_1_2) * percent_x_x100 / 100);
    }
    else
    {
        ret = ((x_1_2 - x_1_1) * percent_x_x100 / 100) + x_1_1;
    }
    return ret;
}
uint16_t LookupTable2d_int8_uint16(int8_t val, s_table_2d_int8_uint16 const *table)
{
    s_scale_idx x = LookupScale_int8(val, table->sc, table->sc_count);
    int x_1_1 = table->table[x.idx1];
    int x_1_2 = table->table[x.idx2];
    int percent_x_x100 = (x.idx1 == x.idx2) ? 0 : ((val - table->sc[x.idx1]) * 100) / (table->sc[x.idx2] - table->sc[x.idx1]);
    uint16_t ret = 0;
    if (x_1_2 < x_1_1)
    {
        ret = x_1_1 - ((x_1_1 - x_1_2) * percent_x_x100 / 100);
    }
    else
    {
        ret = ((x_1_2 - x_1_1) * percent_x_x100 / 100) + x_1_1;
    }
    return ret;
}

uint16_t LookupTable2d_int16_uint16(int16_t val, s_table_2d_int16_uint16 const *table)
{
    s_scale_idx x = LookupScale_int16(val, table->sc, table->sc_count);
    int x_1_1 = table->table[x.idx1];
    int x_1_2 = table->table[x.idx2];
    int percent_x_x100 = (x.idx1 == x.idx2) ? 0 : ((val - table->sc[x.idx1]) * 100) / (table->sc[x.idx2] - table->sc[x.idx1]);
    uint16_t ret = 0;
    if (x_1_2 < x_1_1)
    {
        ret = x_1_1 - ((x_1_1 - x_1_2) * percent_x_x100 / 100);
    }
    else
    {
        ret = ((x_1_2 - x_1_1) * percent_x_x100 / 100) + x_1_1;
    }
    return ret;
}

uint16_t LookupTable2d_uint16_uint16(uint16_t val, s_table_2d_uint16_uint16 const *table)
{
    s_scale_idx x = LookupScale_uint16(val, table->sc, table->sc_count);
    int x_1_1 = table->table[x.idx1];
    int x_1_2 = table->table[x.idx2];
    int percent_x_x100 = (x.idx1 == x.idx2) ? 0 : ((val - table->sc[x.idx1]) * 100) / (table->sc[x.idx2] - table->sc[x.idx1]);
    uint16_t ret = 0;
    if (x_1_2 < x_1_1)
    {
        ret = x_1_1 - ((x_1_1 - x_1_2) * percent_x_x100 / 100);
    }
    else
    {
        ret = ((x_1_2 - x_1_1) * percent_x_x100 / 100) + x_1_1;
    }
    return ret;
}

uint32_t LookupTable2d_uint16_uint32(uint16_t val, s_table_2d_uint16_uint32 const *table)
{
    s_scale_idx x = LookupScale_uint16(val, table->sc, table->sc_count);
    int x_1_1 = table->table[x.idx1];
    int x_1_2 = table->table[x.idx2];
    int percent_x_x100 = (x.idx1 == x.idx2) ? 0 : ((val - table->sc[x.idx1]) * 100) / (table->sc[x.idx2] - table->sc[x.idx1]);
    uint32_t ret = 0;
    if (x_1_2 < x_1_1)
    {
        ret = x_1_1 - ((x_1_1 - x_1_2) * percent_x_x100 / 100);
    }
    else
    {
        ret = ((x_1_2 - x_1_1) * percent_x_x100 / 100) + x_1_1;
    }
    return ret;
}

uint16_t LookupTable2d_uint8_uint8(uint8_t val, s_table_2d_uint8_uint8 const *table)
{
    s_scale_idx x = LookupScale_uint8(val, table->sc, table->sc_count);
    int x_1_1 = table->table[x.idx1];
    int x_1_2 = table->table[x.idx2];
    int percent_x_x100 = (x.idx1 == x.idx2) ? 0 : ((val - table->sc[x.idx1]) * 100) / (table->sc[x.idx2] - table->sc[x.idx1]);
    uint16_t ret = 0;
    if (x_1_2 < x_1_1)
    {
        ret = x_1_1 - ((x_1_1 - x_1_2) * percent_x_x100 / 100);
    }
    else
    {
        ret = ((x_1_2 - x_1_1) * percent_x_x100 / 100) + x_1_1;
    }
    return ret;
}

uint16_t LookupTable2d_uint16_uint8(uint16_t val, s_table_2d_uint16_uint8 const *table)
{
    s_scale_idx x = LookupScale_uint16(val, table->sc, table->sc_count);
    int x_1_1 = table->table[x.idx1];
    int x_1_2 = table->table[x.idx2];
    int percent_x_x100 = (x.idx1 == x.idx2) ? 0 : ((val - table->sc[x.idx1]) * 100) / (table->sc[x.idx2] - table->sc[x.idx1]);
    uint16_t ret = 0;
    if (x_1_2 < x_1_1)
    {
        ret = x_1_1 - ((x_1_1 - x_1_2) * percent_x_x100 / 100);
    }
    else
    {
        ret = ((x_1_2 - x_1_1) * percent_x_x100 / 100) + x_1_1;
    }
    return ret;
}

uint16_t LookupTable3d_uint16_uint16_uint16(uint16_t val_x, uint16_t val_y, s_table_3d_uint16_uint16_uint16 const *table)
{
    s_scale_idx x = LookupScale_uint16(val_x, table->sc_x, table->sc_x_count);
    s_scale_idx y = LookupScale_uint16(val_y, table->sc_y, table->sc_y_count);

    // X-axis by 1st Y axis
    int x_1_1 = table->table[(y.idx1 * table->sc_x_count) + x.idx1];
    int x_1_2 = table->table[(y.idx1 * table->sc_x_count) + x.idx2];
    // X-axis by 2nd Y axis
    int x_2_1 = table->table[(y.idx2 * table->sc_x_count) + x.idx1];
    int x_2_2 = table->table[(y.idx2 * table->sc_x_count) + x.idx2];

    int percent_x_x100 = (x.idx1 == x.idx2) ? 0 : ((val_x - table->sc_x[x.idx1]) * 100) / (table->sc_x[x.idx2] - table->sc_x[x.idx1]);
    int percent_y_x100 = (y.idx1 == y.idx2) ? 0 : ((val_y - table->sc_y[y.idx1]) * 100) / (table->sc_y[y.idx2] - table->sc_y[y.idx1]);
    int x1 = 0;
    int x2 = 0;
    uint16_t ret = 0;
    if (x_1_2 < x_1_1)
    {
        x1 = x_1_1 - ((x_1_1 - x_1_2) * percent_x_x100 / 100);
    }
    else
    {
        x1 = ((x_1_2 - x_1_1) * percent_x_x100 / 100) + x_1_1;
    }
    if (x_2_2 < x_2_1)
    {
        x2 = x_2_1 - ((x_2_1 - x_2_2) * percent_x_x100 / 100);
    }
    else
    {
        x2 = ((x_2_2 - x_2_1) * percent_x_x100 / 100) + x_2_1;
    }
    if (x2 < x1)
    {
        ret = x1 - ((x1 - x2) * percent_y_x100 / 100);
    }
    else
    {
        ret = ((x2 - x1) * percent_y_x100 / 100) + x1;
    }
    return ret;
}

uint16_t LookupTable3d_uint16_uint8_uint16(uint16_t val_x, uint8_t val_y, s_table_3d_uint16_uint8_uint16 const *table)
{
    s_scale_idx x = LookupScale_uint16(val_x, table->sc_x, table->sc_x_count);
    s_scale_idx y = LookupScale_uint8(val_y, table->sc_y, table->sc_y_count);

    // X-axis by 1st Y axis
    int x_1_1 = table->table[(y.idx1 * table->sc_x_count) + x.idx1];
    int x_1_2 = table->table[(y.idx1 * table->sc_x_count) + x.idx2];
    // X-axis by 2nd Y axis
    int x_2_1 = table->table[(y.idx2 * table->sc_x_count) + x.idx1];
    int x_2_2 = table->table[(y.idx2 * table->sc_x_count) + x.idx2];

    int percent_x_x100 = (x.idx1 == x.idx2) ? 0 : ((val_x - table->sc_x[x.idx1]) * 100) / (table->sc_x[x.idx2] - table->sc_x[x.idx1]);
    int percent_y_x100 = (y.idx1 == y.idx2) ? 0 : ((val_y - table->sc_y[y.idx1]) * 100) / (table->sc_y[y.idx2] - table->sc_y[y.idx1]);
    int x1 = 0;
    int x2 = 0;
    uint16_t ret = 0;
    if (x_1_2 < x_1_1)
    {
        x1 = x_1_1 - ((x_1_1 - x_1_2) * percent_x_x100 / 100);
    }
    else
    {
        x1 = ((x_1_2 - x_1_1) * percent_x_x100 / 100) + x_1_1;
    }
    if (x_2_2 < x_2_1)
    {
        x2 = x_2_1 - ((x_2_1 - x_2_2) * percent_x_x100 / 100);
    }
    else
    {
        x2 = ((x_2_2 - x_2_1) * percent_x_x100 / 100) + x_2_1;
    }
    if (x2 < x1)
    {
        ret = x1 - ((x1 - x2) * percent_y_x100 / 100);
    }
    else
    {
        ret = ((x2 - x1) * percent_y_x100 / 100) + x1;
    }
    return ret;
}