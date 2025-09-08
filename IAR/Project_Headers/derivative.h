/*
 * Note: This file is recreated by the project wizard whenever the MCU is
 *       changed and should not be edited by hand
 */

/* Include the derivative-specific header file */
#include <SKEAZN642.h>
#include "mlx75308.h"
#define SIZEOF_ARR(X) (sizeof(X) / sizeof(X[0]))

extern uint32_t SystTick;
extern unsigned char BatteryVoltageLin_x10;
extern short VehicleSpeed;
extern unsigned char dac_level_ir_ch_a;
extern unsigned char dac_level_ir_ch_b;
extern unsigned short IR_Channel_A_data[6];
extern unsigned short IR_Channel_B_data[6];
extern unsigned short Ambient_light_channel_data[TOTAL_AMB_LIGHT_CHANNELS];
extern unsigned int AmbientLIghtLevelsScaled[TOTAL_AMB_LIGHT_CHANNELS];