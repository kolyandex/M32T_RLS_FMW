#include "lin_data.h"
#include "intrinsics.h"
#include "lin.h"
#include "derivative.h"
#include "a_wipe.h"

/**
 * @brief 0-3
 *
 */
unsigned char AutoWipersThresold = 0;
e_wipers_lever_pos WipersSwPos = W_OFF;
e_light_sw_pos LightSwPos = LSW_OFF;
e_ign_state IgnState = IGN_ON;
bool RainDetectedCloseWindowsRequest = false;
bool WipersInOperationNow = false;
bool WasherInOperationNow = false;
unsigned char AllWindowsAreClosed = 0;
short VehicleSpeed = 0;
unsigned char BatteryVoltageLin_x10;

static unsigned char auto_wipers_thresold_prev = 0;
static e_wipers_lever_pos wipers_sw_pos_prev = W_OFF;
static e_wipers_mode wipers_mode = WM_OFF;
static bool a_wipers_enable = true;
static unsigned int one_time_period_ms = 0;
static unsigned int one_time_start_time_ms = 0;

/**
 * @brief lin_wipers_enable
 *
 * @param percent (0-40% - 8 sec - 2 sec, 40-80 - slow, 80-100 - fast)
 */
void lin_wipers_enable(int percent)
{
    one_time_period_ms = 0;
    if (percent < 0)
    {
        percent = 0;
    }
    if (percent > 100)
    {
        percent = 100;
    }
    if (percent > 80)
    {
        wipers_mode = WM_FAST;
    }
    else if (percent > 40)
    {
        wipers_mode = WM_SLOW;
    }
    else if (percent >= 5)
    {
        one_time_period_ms = 8000 - (percent * 15 / 10 * 100); // 8000 (8sec) - 2000 (2sec)
    }
    else
    {
        wipers_mode = WM_OFF;
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

    if (one_time_period_ms != 0)
    {
        if (one_time_start_time_ms == 0)
        {
            one_time_start_time_ms = SystTick;
        }
        if ((SystTick - one_time_start_time_ms) < 500)
        {
            wipers_mode = WM_1_TIME;
        }
        else
        {
            wipers_mode = WM_OFF;
        }
        if (SystTick > (one_time_start_time_ms + one_time_period_ms))
        {
            one_time_start_time_ms = 0;
        }
    }

    // RainDetectedCloseWindowsRequest flag works ony if IGN_OFF (why, chery, why?)
    if (IgnState != IGN_OFF)
    {
        RainDetectedCloseWindowsRequest = false;
    }

    if (AllWindowsAreClosed)
    {
        RainDetectedCloseWindowsRequest = false;
    }

    if ((WipersSwPos != W_AUTO) || (IgnState != IGN_ON))
    {
        wipers_mode = WM_OFF;
    }

    l_u8_wr_LI0_RLS_FrontWipersMode(wipers_mode);
    l_bool_wr_LI0_RLS_FrontWipersAutoModeEnabled(a_wipers_enable);

    l_bool_wr_LI0_RLS_RainDetectedCloseWindows(RainDetectedCloseWindowsRequest);
    l_u8_wr_LI0_RLS_AmbientLightLevel_pow(amb_pow);
    l_u8_wr_LI0_RLS_AmbientLightLevel(amb_lvl);
    l_u8_wr_LI0_RLS_AutoLightOn_0(TurnOnLights);
    l_bool_wr_LI0_RLS_AutoLightOn_1(TurnOnLights);
    if (TurnOnLights)
    {
        if (IsTooDark)
        {
            l_u8_wr_LI0_RLS_TooDarkOutside(3);
        }
        else
        {
            l_u8_wr_LI0_RLS_TooDarkOutside(2);
        }
    }
    else
    {
        l_u8_wr_LI0_RLS_TooDarkOutside(0);
    }

    BatteryVoltageLin_x10 = l_u8_rd_LI0_BCM_BatteryVoltage();
    VehicleSpeed = l_u16_rd_LI0_BCM_VehicleSpeed() * 3 / 40;
    WipersSwPos = (e_wipers_lever_pos)l_u8_rd_LI0_BCM_WipersSwPos();
    LightSwPos = (e_light_sw_pos)l_u8_rd_LI0_BCM_LightSwitchPos();
    IgnState = (e_ign_state)l_u8_rd_LI0_BCM_IgnState();
    WipersInOperationNow = l_bool_rd_LI0_BCM_WipersInOperationNow();
    AutoWipersThresold = l_u8_rd_LI0_BCM_AutoWipersThreshold();
    WasherInOperationNow = ((l_u8_rd_LI0_BCM_Washer() & 1) == 1);
    AllWindowsAreClosed = l_u8_rd_LI0_BCM_AllWindowsClosedFlag();

    if (AutoWipersThresold > (AUTO_WIPERS_THRESOLD_MODES_COUNT - 1))
    {
        AutoWipersThresold = (AUTO_WIPERS_THRESOLD_MODES_COUNT - 1);
    }

    if ((wipers_sw_pos_prev != WipersSwPos) || (auto_wipers_thresold_prev != AutoWipersThresold))
    {
        if ((WipersSwPos == W_AUTO) && (IgnState == IGN_ON) && (wipers_mode == WM_OFF))
        {
            l_u8_wr_LI0_RLS_FrontWipersMode(WM_1_TIME);
        }
    }
    auto_wipers_thresold_prev = AutoWipersThresold;
    wipers_sw_pos_prev = WipersSwPos;

    l_u8_wr_LI0_RLS_8E_4_CAN_355_4(IR_Channel_A_data >> 8);
    l_u8_wr_LI0_RLS_8E_5_CAN_355_3(IR_Channel_A_data >> 0);
    l_u8_wr_LI0_RLS_8E_6_CAN_355_1(IR_Channel_B_data >> 8);
    l_u8_wr_LI0_RLS_8E_7_CAN_355_2(IR_Channel_B_data >> 0);

    __enable_interrupt();
}