#include "lin_data.h"
#include "intrinsics.h"
#include "lin.h"
#include "derivative.h"

e_wipers_lever_pos WipersSwPos = W_OFF;
e_light_sw_pos LightSwPos = LSW_OFF;
e_ign_state IgnState = IGN_ON;
bool RainDetectedCloseWindowsRequest = 0;
bool WipersInOperationNow = false;

static e_wipers_lever_pos wipers_sw_pos_prev = W_OFF;
static unsigned int wipers_mode_counter = 0;
static e_wipers_mode wipers_mode = WM_OFF;
static bool a_wipers_enable = true;


typedef struct
{
    e_wipers_mode current_mode;
    e_wipers_mode new_mode;
} s_w_mode_relations;

static const s_w_mode_relations w_mode_relations[] =
    {
        {.current_mode = WM_OFF, .new_mode = WM_1_TIME},
        {.current_mode = WM_OFF, .new_mode = WM_SLOW},
        {.current_mode = WM_OFF, .new_mode = WM_FAST},
        {.current_mode = WM_1_TIME, .new_mode = WM_SLOW},
        {.current_mode = WM_1_TIME, .new_mode = WM_FAST},
        {.current_mode = WM_SLOW, .new_mode = WM_FAST},
        {.current_mode = WM_FAST, .new_mode = WM_SLOW},
        {.current_mode = WM_SLOW, .new_mode = WM_OFF},
        {.current_mode = WM_FAST, .new_mode = WM_OFF},
};

void lin_wipers_set_mode(e_wipers_mode mode, unsigned int time)
{
    if (mode == WM_1_TIME)
    {
        time = 5;
    }
    for (int i = 0; i < SIZEOF_ARR(w_mode_relations); i++)
    {
        if (((w_mode_relations[i].current_mode == wipers_mode) && (w_mode_relations[i].new_mode == mode)) || (wipers_mode == mode))
        {
            wipers_mode = mode;
            wipers_mode_counter = time;
            break;
        }
    }
}

void lin_proc_data_100ms(void)
{
    __disable_interrupt();
    uint32_t amb_lvl = AmbientLIghtLevelsScaled[0];
    uint8_t amb_pow = 0;
    if (amb_lvl > 127)
    {
        if (amb_lvl > 1270)
        {
            if (amb_lvl > 12700)
            {
                amb_pow = 3;
                if (amb_lvl > 127000)
                {
                    amb_lvl = INT8_MAX;
                }
                else
                {
                    amb_lvl /= 1000;
                }
            }
            else
            {
                amb_pow = 2;
                amb_lvl /= 100;
            }
        }
        else
        {
            amb_pow = 1;
            amb_lvl /= 10;
        }
    }

    if (wipers_mode_counter)
    {
        wipers_mode_counter--;
    }
    else
    {
        wipers_mode = WM_OFF;
    }

    if (IgnState >= IGN_ON)
    {
        RainDetectedCloseWindowsRequest = 0;
    }

    if (WipersSwPos != W_AUTO)
    {
        wipers_mode_counter = 0;
        wipers_mode = WM_OFF;
    }

    l_u8_wr_LI0_RLS_FrontWipersMode(wipers_mode);
    l_bool_wr_LI0_RLS_FrontWipersAutoModeEnabled(a_wipers_enable);

    l_bool_wr_LI0_RLS_RainDetectedCloseWindows(RainDetectedCloseWindowsRequest);
    l_u8_wr_LI0_RLS_AmbientLightLevel_pow(amb_pow);
    l_u8_wr_LI0_RLS_AmbientLightLevel(amb_lvl);
    l_u8_wr_LI0_RLS_AutoLightOn_0(TurnOnLights);
    l_bool_wr_LI0_RLS_AutoLightOn_1(TurnOnLights);

    BatteryVoltageLin_x10 = l_u8_rd_LI0_BCM_BatteryVoltage();
    VehicleSpeed = l_u16_rd_LI0_BCM_VehicleSpeed() * 3 / 40;
    WipersSwPos = l_u8_rd_LI0_BCM_WipersSwPos();
    LightSwPos = l_u8_rd_LI0_BCM_LightSwitchPos();
    IgnState = l_u8_rd_LI0_BCM_IgnState();
    WipersInOperationNow = l_bool_rd_LI0_BCM_WipersInOperationNow();

    if (wipers_sw_pos_prev != WipersSwPos)
    {
        if (WipersSwPos == W_AUTO)
        {
            lin_wipers_set_mode(WM_1_TIME, 0);
        }
    }

    wipers_sw_pos_prev = WipersSwPos;
    __enable_interrupt();
}