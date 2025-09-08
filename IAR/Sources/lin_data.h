#pragma once

typedef enum
{
    W_OFF = 0,
    W_AUTO,
    W_ON_SLOW,
    W_ON_FAST
} e_wipers_lever_pos;
typedef enum
{
    LSW_OFF = 0,
    LSW_AUTO,
    LSW_TAILLIGHTS,
    LSW_ON
} e_light_sw_pos;

void lin_proc_data_100ms(void);