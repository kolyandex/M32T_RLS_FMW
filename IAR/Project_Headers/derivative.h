/*
 * Note: This file is recreated by the project wizard whenever the MCU is
 *       changed and should not be edited by hand
 */

/* Include the derivative-specific header file */
#include <SKEAZN642.h>
#include "mlx75308.h"
#include "lin_data.h"
#include "stdbool.h"
#define SIZEOF_ARR(X) (sizeof(X) / sizeof(X[0]))


#define OUTPUT 1
#define INPUT 0

#define PORT_A A
#define PORT_B A
#define PORT_C A
#define PORT_D A

#define PORT_E B
#define PORT_F B
#define PORT_G B
#define PORT_H B

#define CONFIG_PIN_AS_GPIO(port, register_number, mode) XCONFIG_PIN_AS_GPIO(port, register_number, mode)
#define XCONFIG_PIN_AS_GPIO(port, register_number, mode) (mode == 0) ? (GPIO##port##_PDDR &= ~(1 << register_number)) : (GPIO##port##_PDDR |= 1 << register_number)

#define ENABLE_INPUT(port, pin_number) XENABLE_INPUT(port, pin_number)
#define XENABLE_INPUT(port, pin_number) GPIO##port##_PIDR &= ~(1 << pin_number)

#define ENABLE_PULLUP(port, pin_number) XENABLE_PULLUP(port, pin_number)
#define XENABLE_PULLUP(port, pin_number) PORT_PUE0 |= PORT_PUE0_PT##port##PE##pin_number##_MASK

#define OUTPUT_SET(port, register_num) XOUTPUT_SET(port, register_num)
#define XOUTPUT_SET(port, register_num) GPIO##port##_PSOR |= 1 << register_num

#define OUTPUT_CLEAR(port, register_num) XOUTPUT_CLEAR(port, register_num)
#define XOUTPUT_CLEAR(port, register_num) GPIO##port##_PCOR |= 1 << register_num

#define OUTPUT_TOGGLE(port, register_num) XOUTPUT_TOGGLE(port, register_num)
#define XOUTPUT_TOGGLE(port, register_num) GPIO##port##_PTOR |= 1 << register_num

extern uint32_t SystTick;
extern unsigned char BatteryVoltageLin_x10;
extern short VehicleSpeed;
extern unsigned char dac_level_ir_ch_a;
extern unsigned char dac_level_ir_ch_b;
extern unsigned short IR_Channel_A_data[6];
extern unsigned short IR_Channel_B_data[6];
extern unsigned short Ambient_light_channel_data[TOTAL_AMB_LIGHT_CHANNELS];
extern unsigned int AmbientLIghtLevelsScaled[TOTAL_AMB_LIGHT_CHANNELS];
extern unsigned char TurnOnLights;
extern unsigned short l_long_avg;
extern unsigned short l_short_avg;
extern e_wipers_lever_pos WipersSwPos;
extern e_light_sw_pos LightSwPos;
extern bool RainDetectedCloseWindowsRequest;
extern bool WipersInOperationNow;



void lin_application_timer_FTM2();