#include "lin_data.h"
#include "intrinsics.h"
#include "lin.h"
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

    l_u8_wr_LI0_RLS_AmbientLightLevel_pow(amb_pow);
    l_u8_wr_LI0_RLS_AmbientLightLevel(amb_lvl);



    BatteryVoltageLin_x10 = l_u8_rd_LI0_BCM_BatteryVoltage();
    VehicleSpeed = l_u16_rd_LI0_BCM_VehicleSpeed() * 3 / 40;

    __enable_interrupt();
}