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
typedef enum
{
    IGN_OFF = 0,
    IGN_ACC = 1,
    IGN_ON = 2,
    IGN_ENGINE_START_PROCESS = 3
} e_ign_state;

typedef enum
{
    WM_OFF = 0,
    WM_1_TIME = 1,
    WM_SLOW = 2,
    WM_FAST = 3
} e_wipers_mode;

void lin_proc_data_100ms(void);
void lin_wipers_set_mode(e_wipers_mode mode, unsigned int time);