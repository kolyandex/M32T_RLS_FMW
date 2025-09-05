/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2013-2016 Freescale Semiconductor, Inc.
* Copyright 2016-2025 NXP
* ALL RIGHTS RESERVED.
*
****************************************************************************//*!
*
* @file      lin_cfg.h
*
* @author    FPT Software
*
* @version   1.0
*
* @date      Fri Sep 05 09:53:30 NOVT 2025
*
* @brief     Hardware configuration file
*
******************************************************************************/
#ifndef    _LIN_CFG_H_
#define    _LIN_CFG_H_
#include "lin_hw_cfg.h"
/* Define operating mode */
#define _MASTER_MODE_     0
#define _SLAVE_MODE_      1
#define LIN_MODE   _SLAVE_MODE_
/* Define protocol version */
#define PROTOCOL_21       0
#define PROTOCOL_J2602    1
#define PROTOCOL_20       2
#define LIN_PROTOCOL    PROTOCOL_21

#define UART_ADDR        UART0_ADDR    /* For slave */


#define LIN_BAUD_RATE    19200         /*For slave*/
/**********************************************************************/
/***************          Diagnostic class selection  *****************/
/**********************************************************************/
#define _DIAG_CLASS_I_          0
#define _DIAG_CLASS_II_         1
#define _DIAG_CLASS_III_        2

#define _DIAG_CLASS_SUPPORT_    _DIAG_CLASS_II_

#define MAX_LENGTH_SERVICE 120

#define MAX_QUEUE_SIZE 21


#define _DIAG_NUMBER_OF_SERVICES_    8

#define DIAGSRV_READ_BY_IDENTIFIER_ORDER    0

#define DIAGSRV_ASSIGN_FRAME_ID_RANGE_ORDER    1

#define DIAGSRV_READ_DATA_BY_IDENTIFIER_ORDER    2

#define DIAGSRV_SAVE_CONFIGURATION_ORDER    3

#define DIAGSRV_DATA_DUMP_ORDER    4

#define DIAGSRV_CONDITIONAL_CHANGE_NAD_ORDER    5

#define DIAGSRV_ASSIGN_NAD_ORDER    6

#define DIAGSRV_WRITE_DATA_BY_IDENTIFIER_ORDER    7


/**************** FRAME SUPPORT DEFINITION ******************/
#define _TL_SINGLE_FRAME_       0
#define _TL_MULTI_FRAME_        1

#define _TL_FRAME_SUPPORT_      _TL_MULTI_FRAME_

/* frame buffer size */
#define LIN_FRAME_BUF_SIZE          40
#define LIN_FLAG_BUF_SIZE           6

/**********************************************************************/
/***************               Interfaces           *******************/
/**********************************************************************/
typedef enum {
   LI0
}l_ifc_handle;

/**********************************************************************/
/***************               Signals              *******************/
/**********************************************************************/
/* Number of signals */
#define LIN_NUM_OF_SIGS  23
/* List of signals */
typedef enum {

   /* Interface_name = LI0 */

   LI0_BCM_AutoWipersThreshold

   , LI0_BCM_AmbientLightOn
  
   , LI0_RLS_FrontWipersMode
  
   , LI0_SIG_Error
  
   , LI0_RLS_FrontWipersAutoModeEnabled
  
   , LI0_RLS_AutoLightOn_0
  
   , LI0_RLS_AutoLightOn_1
  
   , LI0_RLS_Unknown_1
  
   , LI0_RLS_AmbientLightLevel_0_3
  
   , LI0_RLS_Unknown_0
  
   , LI0_RLS_Unknown_2
  
   , LI0_RLS_AmbientLightLevel
  
   , LI0_BCM_WipersSwPos
  
   , LI0_BCM_Washer
  
   , LI0_BCM_LightSwitchPos
  
   , LI0_BCM_Unknown
  
   , LI0_BCM_VehicleSpeed_L
  
   , LI0_BCM_VehicleSpeed_H
  
   , LI0_BCM_BatteryVoltage
  
   , LI0_RLS_LinError
  
   , LI0_BCM_11_Unk_0
  
   , LI0_BCM_11_Unk_1
  
   , LI0_BCM_11_Unk_2
  
} l_signal_handle;
/**********************************************************************/
/*****************               Frame             ********************/
/**********************************************************************/
/* Number of frames */
#define LIN_NUM_OF_FRMS  7
/* List of frames */
typedef enum {
/* All frames for master node */

   /* Interface_name = LI0 */

   LI0_BCM_55

   , LI0_RLS_8E
  
   , LI0_BCM_D6
  
   , LI0_BCM_85
  
   , LI0_BCM_11
  
   , LI0_MasterReq
  
   , LI0_SlaveResp
  
} l_frame_handle;
/**********************************************************************/
/***************             Configuration          *******************/
/**********************************************************************/
/* Size of configuration in ROM and RAM used for interface: LI1 */
#define LIN_SIZE_OF_CFG  9
#define LIN_CFG_FRAME_NUM  5
/*********************************************************************
 * global macros
 *********************************************************************/
#define l_bool_rd(SIGNAL) l_bool_rd_##SIGNAL()
#define l_bool_wr(SIGNAL, A) l_bool_wr_##SIGNAL(A)
#define l_u8_rd(SIGNAL) l_u8_rd_##SIGNAL()
#define l_u8_wr(SIGNAL, A) l_u8_wr_##SIGNAL(A)
#define l_u16_rd(SIGNAL) l_u16_rd_##SIGNAL()
#define l_u16_wr(SIGNAL, A) l_u16_wr_##SIGNAL(A)
#define l_bytes_rd(SIGNAL, start, count, data)  l_bytes_rd_##SIGNAL(start, count, data)
#define l_bytes_wr(SIGNAL, start, count, data) l_bytes_wr_##SIGNAL(start, count, data)
#define l_flg_tst(FLAG) l_flg_tst_##FLAG()
#define l_flg_clr(FLAG) l_flg_clr_##FLAG()
#define LIN_TEST_BIT(A,B) ((l_bool)((((A) & (1U << (B))) != 0U) ? 1U : 0U))
#define LIN_SET_BIT(A,B)                      ((A) |= (l_u8) (1U << (B)))
#define LIN_CLEAR_BIT(A,B)               ((A) &= ((l_u8) (~(1U << (B)))))
#define LIN_BYTE_MASK  ((l_u16)(((l_u16)((l_u16)1 << CHAR_BIT)) - (l_u16)1))
#define LIN_FRAME_LEN_MAX                                             10U

/* Returns the low byte of the 32-bit value    */
#define BYTE_0(n)                              ((l_u8)((n) & (l_u8)0xFF))
/* Returns the second byte of the 32-bit value */
#define BYTE_1(n)                        ((l_u8)(BYTE_0((n) >> (l_u8)8)))
/* Returns the third byte of the 32-bit value  */
#define BYTE_2(n)                       ((l_u8)(BYTE_0((n) >> (l_u8)16)))
/* Returns high byte of the 32-bit value       */
#define BYTE_3(n)                       ((l_u8)(BYTE_0((n) >> (l_u8)24)))

/*
 * defines for signal access
 */



#define LIN_BYTE_OFFSET_LI0_BCM_AutoWipersThreshold    0U
#define LIN_BIT_OFFSET_LI0_BCM_AutoWipersThreshold    0U
#define LIN_SIGNAL_SIZE_LI0_BCM_AutoWipersThreshold    3U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_AutoWipersThreshold    0U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_AutoWipersThreshold    0U


#define LIN_BYTE_OFFSET_LI0_BCM_AmbientLightOn    16U
#define LIN_BIT_OFFSET_LI0_BCM_AmbientLightOn    2U
#define LIN_SIGNAL_SIZE_LI0_BCM_AmbientLightOn    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_AmbientLightOn    3U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_AmbientLightOn    0U


#define LIN_BYTE_OFFSET_LI0_RLS_FrontWipersMode    8U
#define LIN_BIT_OFFSET_LI0_RLS_FrontWipersMode    6U
#define LIN_SIGNAL_SIZE_LI0_RLS_FrontWipersMode    2U
#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_FrontWipersMode    1U
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_FrontWipersMode    5U


#define LIN_BYTE_OFFSET_LI0_SIG_Error    8U
#define LIN_BIT_OFFSET_LI0_SIG_Error    4U
#define LIN_SIGNAL_SIZE_LI0_SIG_Error    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_SIG_Error    1U
#define LIN_FLAG_BIT_OFFSET_LI0_SIG_Error    3U


#define LIN_BYTE_OFFSET_LI0_RLS_FrontWipersAutoModeEnabled    8U
#define LIN_BIT_OFFSET_LI0_RLS_FrontWipersAutoModeEnabled    5U
#define LIN_SIGNAL_SIZE_LI0_RLS_FrontWipersAutoModeEnabled    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_FrontWipersAutoModeEnabled    1U
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_FrontWipersAutoModeEnabled    4U


#define LIN_BYTE_OFFSET_LI0_RLS_AutoLightOn_0    8U
#define LIN_BIT_OFFSET_LI0_RLS_AutoLightOn_0    1U
#define LIN_SIGNAL_SIZE_LI0_RLS_AutoLightOn_0    2U
#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_AutoLightOn_0    1U
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_AutoLightOn_0    1U


#define LIN_BYTE_OFFSET_LI0_RLS_AutoLightOn_1    8U
#define LIN_BIT_OFFSET_LI0_RLS_AutoLightOn_1    3U
#define LIN_SIGNAL_SIZE_LI0_RLS_AutoLightOn_1    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_AutoLightOn_1    1U
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_AutoLightOn_1    2U


#define LIN_BYTE_OFFSET_LI0_RLS_Unknown_1    8U
#define LIN_BIT_OFFSET_LI0_RLS_Unknown_1    0U
#define LIN_SIGNAL_SIZE_LI0_RLS_Unknown_1    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_Unknown_1    1U
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_Unknown_1    0U


#define LIN_BYTE_OFFSET_LI0_RLS_AmbientLightLevel_0_3    9U
#define LIN_BIT_OFFSET_LI0_RLS_AmbientLightLevel_0_3    2U
#define LIN_SIGNAL_SIZE_LI0_RLS_AmbientLightLevel_0_3    2U
#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_AmbientLightLevel_0_3    1U
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_AmbientLightLevel_0_3    7U


#define LIN_BYTE_OFFSET_LI0_RLS_Unknown_0    9U
#define LIN_BIT_OFFSET_LI0_RLS_Unknown_0    5U
#define LIN_SIGNAL_SIZE_LI0_RLS_Unknown_0    2U
#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_Unknown_0    2U
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_Unknown_0    1U


#define LIN_BYTE_OFFSET_LI0_RLS_Unknown_2    9U
#define LIN_BIT_OFFSET_LI0_RLS_Unknown_2    1U
#define LIN_SIGNAL_SIZE_LI0_RLS_Unknown_2    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_Unknown_2    1U
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_Unknown_2    6U


#define LIN_BYTE_OFFSET_LI0_RLS_AmbientLightLevel    10U
#define LIN_BIT_OFFSET_LI0_RLS_AmbientLightLevel    1U
#define LIN_SIGNAL_SIZE_LI0_RLS_AmbientLightLevel    7U
#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_AmbientLightLevel    2U
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_AmbientLightLevel    2U


#define LIN_BYTE_OFFSET_LI0_BCM_WipersSwPos    0U
#define LIN_BIT_OFFSET_LI0_BCM_WipersSwPos    5U
#define LIN_SIGNAL_SIZE_LI0_BCM_WipersSwPos    3U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_WipersSwPos    0U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_WipersSwPos    2U


#define LIN_BYTE_OFFSET_LI0_BCM_Washer    0U
#define LIN_BIT_OFFSET_LI0_BCM_Washer    3U
#define LIN_SIGNAL_SIZE_LI0_BCM_Washer    2U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_Washer    0U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_Washer    1U


#define LIN_BYTE_OFFSET_LI0_BCM_LightSwitchPos    1U
#define LIN_BIT_OFFSET_LI0_BCM_LightSwitchPos    5U
#define LIN_SIGNAL_SIZE_LI0_BCM_LightSwitchPos    3U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_LightSwitchPos    0U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_LightSwitchPos    4U


#define LIN_BYTE_OFFSET_LI0_BCM_Unknown    1U
#define LIN_BIT_OFFSET_LI0_BCM_Unknown    4U
#define LIN_SIGNAL_SIZE_LI0_BCM_Unknown    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_Unknown    0U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_Unknown    3U


#define LIN_BYTE_OFFSET_LI0_BCM_VehicleSpeed_L    28U
#define LIN_BIT_OFFSET_LI0_BCM_VehicleSpeed_L    0U
#define LIN_SIGNAL_SIZE_LI0_BCM_VehicleSpeed_L    4U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_VehicleSpeed_L    4U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_VehicleSpeed_L    0U


#define LIN_BYTE_OFFSET_LI0_BCM_VehicleSpeed_H    29U
#define LIN_BIT_OFFSET_LI0_BCM_VehicleSpeed_H    0U
#define LIN_SIGNAL_SIZE_LI0_BCM_VehicleSpeed_H    8U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_VehicleSpeed_H    4U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_VehicleSpeed_H    1U


#define LIN_BYTE_OFFSET_LI0_BCM_BatteryVoltage    31U
#define LIN_BIT_OFFSET_LI0_BCM_BatteryVoltage    0U
#define LIN_SIGNAL_SIZE_LI0_BCM_BatteryVoltage    8U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_BatteryVoltage    4U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_BatteryVoltage    2U


#define LIN_BYTE_OFFSET_LI0_RLS_LinError    9U
#define LIN_BIT_OFFSET_LI0_RLS_LinError    4U
#define LIN_SIGNAL_SIZE_LI0_RLS_LinError    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_LinError    2U
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_LinError    0U


#define LIN_BYTE_OFFSET_LI0_BCM_11_Unk_0    36U
#define LIN_BIT_OFFSET_LI0_BCM_11_Unk_0    6U
#define LIN_SIGNAL_SIZE_LI0_BCM_11_Unk_0    2U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_11_Unk_0    5U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_11_Unk_0    2U


#define LIN_BYTE_OFFSET_LI0_BCM_11_Unk_1    36U
#define LIN_BIT_OFFSET_LI0_BCM_11_Unk_1    5U
#define LIN_SIGNAL_SIZE_LI0_BCM_11_Unk_1    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_11_Unk_1    5U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_11_Unk_1    1U


#define LIN_BYTE_OFFSET_LI0_BCM_11_Unk_2    34U
#define LIN_BIT_OFFSET_LI0_BCM_11_Unk_2    0U
#define LIN_SIGNAL_SIZE_LI0_BCM_11_Unk_2    8U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_11_Unk_2    5U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_11_Unk_2    0U




#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_55             0
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_55              0

#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_8E             1
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_8E              0

#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_D6             3
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_D6              0

#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_85             4
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_85              0

#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_11             5
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_11              0


/**********************************************************************/
/***************        Static API Functions        *******************/
/**********************************************************************/
/*
 * the static signal access macros
 */


 
/* static access macros for signal LI0_BCM_AutoWipersThreshold */
 
#define l_u8_rd_LI0_BCM_AutoWipersThreshold() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_AutoWipersThreshold]) >> 0U) & 0x07U))


#define l_u8_wr_LI0_BCM_AutoWipersThreshold(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_AutoWipersThreshold] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_AutoWipersThreshold] & 0xf8U) | \
    (((A) << 0U) & 0x07U)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_AutoWipersThreshold],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_AutoWipersThreshold); \
    }


/* static access macros for signal LI0_BCM_AmbientLightOn */

 
#define l_bool_rd_LI0_BCM_AmbientLightOn() \
    (LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_AmbientLightOn], \
    LIN_BIT_OFFSET_LI0_BCM_AmbientLightOn))

#define l_bool_wr_LI0_BCM_AmbientLightOn(A) \
    {(A) ? \
    (LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_AmbientLightOn], \
    LIN_BIT_OFFSET_LI0_BCM_AmbientLightOn)):\
    (LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_AmbientLightOn], \
    LIN_BIT_OFFSET_LI0_BCM_AmbientLightOn));\
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_AmbientLightOn],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_AmbientLightOn);}
 
/* static access macros for signal LI0_RLS_FrontWipersMode */
 
#define l_u8_rd_LI0_RLS_FrontWipersMode() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_FrontWipersMode]) >> 6U) & 0x03U))


#define l_u8_wr_LI0_RLS_FrontWipersMode(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_FrontWipersMode] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_FrontWipersMode] & 0x3fU) | \
    (((A) << 6U) & 0xc0U)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_FrontWipersMode],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_FrontWipersMode); \
    }


/* static access macros for signal LI0_SIG_Error */

 
#define l_bool_rd_LI0_SIG_Error() \
    (LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_SIG_Error], \
    LIN_BIT_OFFSET_LI0_SIG_Error))

#define l_bool_wr_LI0_SIG_Error(A) \
    {(A) ? \
    (LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_SIG_Error], \
    LIN_BIT_OFFSET_LI0_SIG_Error)):\
    (LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_SIG_Error], \
    LIN_BIT_OFFSET_LI0_SIG_Error));\
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_SIG_Error],\
         LIN_FLAG_BIT_OFFSET_LI0_SIG_Error);}
/* static access macros for signal LI0_RLS_FrontWipersAutoModeEnabled */

 
#define l_bool_rd_LI0_RLS_FrontWipersAutoModeEnabled() \
    (LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_FrontWipersAutoModeEnabled], \
    LIN_BIT_OFFSET_LI0_RLS_FrontWipersAutoModeEnabled))

#define l_bool_wr_LI0_RLS_FrontWipersAutoModeEnabled(A) \
    {(A) ? \
    (LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_FrontWipersAutoModeEnabled], \
    LIN_BIT_OFFSET_LI0_RLS_FrontWipersAutoModeEnabled)):\
    (LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_FrontWipersAutoModeEnabled], \
    LIN_BIT_OFFSET_LI0_RLS_FrontWipersAutoModeEnabled));\
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_FrontWipersAutoModeEnabled],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_FrontWipersAutoModeEnabled);}
 
/* static access macros for signal LI0_RLS_AutoLightOn_0 */
 
#define l_u8_rd_LI0_RLS_AutoLightOn_0() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AutoLightOn_0]) >> 1U) & 0x03U))


#define l_u8_wr_LI0_RLS_AutoLightOn_0(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AutoLightOn_0] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AutoLightOn_0] & 0xf9U) | \
    (((A) << 1U) & 0x06U)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_AutoLightOn_0],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_AutoLightOn_0); \
    }


/* static access macros for signal LI0_RLS_AutoLightOn_1 */

 
#define l_bool_rd_LI0_RLS_AutoLightOn_1() \
    (LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AutoLightOn_1], \
    LIN_BIT_OFFSET_LI0_RLS_AutoLightOn_1))

#define l_bool_wr_LI0_RLS_AutoLightOn_1(A) \
    {(A) ? \
    (LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AutoLightOn_1], \
    LIN_BIT_OFFSET_LI0_RLS_AutoLightOn_1)):\
    (LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AutoLightOn_1], \
    LIN_BIT_OFFSET_LI0_RLS_AutoLightOn_1));\
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_AutoLightOn_1],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_AutoLightOn_1);}
/* static access macros for signal LI0_RLS_Unknown_1 */

 
#define l_bool_rd_LI0_RLS_Unknown_1() \
    (LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_Unknown_1], \
    LIN_BIT_OFFSET_LI0_RLS_Unknown_1))

#define l_bool_wr_LI0_RLS_Unknown_1(A) \
    {(A) ? \
    (LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_Unknown_1], \
    LIN_BIT_OFFSET_LI0_RLS_Unknown_1)):\
    (LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_Unknown_1], \
    LIN_BIT_OFFSET_LI0_RLS_Unknown_1));\
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_Unknown_1],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_Unknown_1);}
 
/* static access macros for signal LI0_RLS_AmbientLightLevel_0_3 */
 
#define l_u8_rd_LI0_RLS_AmbientLightLevel_0_3() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AmbientLightLevel_0_3]) >> 2U) & 0x03U))


#define l_u8_wr_LI0_RLS_AmbientLightLevel_0_3(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AmbientLightLevel_0_3] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AmbientLightLevel_0_3] & 0xf3U) | \
    (((A) << 2U) & 0x0cU)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_AmbientLightLevel_0_3],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_AmbientLightLevel_0_3); \
    }


 
/* static access macros for signal LI0_RLS_Unknown_0 */
 
#define l_u8_rd_LI0_RLS_Unknown_0() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_Unknown_0]) >> 5U) & 0x03U))


#define l_u8_wr_LI0_RLS_Unknown_0(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_Unknown_0] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_Unknown_0] & 0x9fU) | \
    (((A) << 5U) & 0x60U)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_Unknown_0],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_Unknown_0); \
    }


/* static access macros for signal LI0_RLS_Unknown_2 */

 
#define l_bool_rd_LI0_RLS_Unknown_2() \
    (LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_Unknown_2], \
    LIN_BIT_OFFSET_LI0_RLS_Unknown_2))

#define l_bool_wr_LI0_RLS_Unknown_2(A) \
    {(A) ? \
    (LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_Unknown_2], \
    LIN_BIT_OFFSET_LI0_RLS_Unknown_2)):\
    (LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_Unknown_2], \
    LIN_BIT_OFFSET_LI0_RLS_Unknown_2));\
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_Unknown_2],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_Unknown_2);}
 
/* static access macros for signal LI0_RLS_AmbientLightLevel */
 
#define l_u8_rd_LI0_RLS_AmbientLightLevel() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AmbientLightLevel]) >> 1U) & 0x7fU))


#define l_u8_wr_LI0_RLS_AmbientLightLevel(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AmbientLightLevel] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AmbientLightLevel] & 0x01U) | \
    (((A) << 1U) & 0xfeU)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_AmbientLightLevel],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_AmbientLightLevel); \
    }


 
/* static access macros for signal LI0_BCM_WipersSwPos */
 
#define l_u8_rd_LI0_BCM_WipersSwPos() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_WipersSwPos]) >> 5U) & 0x07U))


#define l_u8_wr_LI0_BCM_WipersSwPos(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_WipersSwPos] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_WipersSwPos] & 0x1fU) | \
    (((A) << 5U) & 0xe0U)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_WipersSwPos],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_WipersSwPos); \
    }


 
/* static access macros for signal LI0_BCM_Washer */
 
#define l_u8_rd_LI0_BCM_Washer() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_Washer]) >> 3U) & 0x03U))


#define l_u8_wr_LI0_BCM_Washer(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_Washer] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_Washer] & 0xe7U) | \
    (((A) << 3U) & 0x18U)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_Washer],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_Washer); \
    }


 
/* static access macros for signal LI0_BCM_LightSwitchPos */
 
#define l_u8_rd_LI0_BCM_LightSwitchPos() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_LightSwitchPos]) >> 5U) & 0x07U))


#define l_u8_wr_LI0_BCM_LightSwitchPos(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_LightSwitchPos] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_LightSwitchPos] & 0x1fU) | \
    (((A) << 5U) & 0xe0U)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_LightSwitchPos],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_LightSwitchPos); \
    }


/* static access macros for signal LI0_BCM_Unknown */

 
#define l_bool_rd_LI0_BCM_Unknown() \
    (LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_Unknown], \
    LIN_BIT_OFFSET_LI0_BCM_Unknown))

#define l_bool_wr_LI0_BCM_Unknown(A) \
    {(A) ? \
    (LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_Unknown], \
    LIN_BIT_OFFSET_LI0_BCM_Unknown)):\
    (LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_Unknown], \
    LIN_BIT_OFFSET_LI0_BCM_Unknown));\
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_Unknown],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_Unknown);}
 
/* static access macros for signal LI0_BCM_VehicleSpeed_L */
 
#define l_u8_rd_LI0_BCM_VehicleSpeed_L() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_VehicleSpeed_L]) >> 0U) & 0x0fU))


#define l_u8_wr_LI0_BCM_VehicleSpeed_L(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_VehicleSpeed_L] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_VehicleSpeed_L] & 0xf0U) | \
    (((A) << 0U) & 0x0fU)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_VehicleSpeed_L],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_VehicleSpeed_L); \
    }


 
/* static access macros for signal LI0_BCM_VehicleSpeed_H */
 
#define l_u8_rd_LI0_BCM_VehicleSpeed_H() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_VehicleSpeed_H]) >> 0U) & 0xffU))


#define l_u8_wr_LI0_BCM_VehicleSpeed_H(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_VehicleSpeed_H] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_VehicleSpeed_H] & 0x00U) | \
    (((A) << 0U) & 0xffU)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_VehicleSpeed_H],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_VehicleSpeed_H); \
    }


 
/* static access macros for signal LI0_BCM_BatteryVoltage */
 
#define l_u8_rd_LI0_BCM_BatteryVoltage() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_BatteryVoltage]) >> 0U) & 0xffU))


#define l_u8_wr_LI0_BCM_BatteryVoltage(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_BatteryVoltage] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_BatteryVoltage] & 0x00U) | \
    (((A) << 0U) & 0xffU)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_BatteryVoltage],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_BatteryVoltage); \
    }


/* static access macros for signal LI0_RLS_LinError */

 
#define l_bool_rd_LI0_RLS_LinError() \
    (LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_LinError], \
    LIN_BIT_OFFSET_LI0_RLS_LinError))

#define l_bool_wr_LI0_RLS_LinError(A) \
    {(A) ? \
    (LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_LinError], \
    LIN_BIT_OFFSET_LI0_RLS_LinError)):\
    (LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_LinError], \
    LIN_BIT_OFFSET_LI0_RLS_LinError));\
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_LinError],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_LinError);}
 
/* static access macros for signal LI0_BCM_11_Unk_0 */
 
#define l_u8_rd_LI0_BCM_11_Unk_0() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_11_Unk_0]) >> 6U) & 0x03U))


#define l_u8_wr_LI0_BCM_11_Unk_0(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_11_Unk_0] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_11_Unk_0] & 0x3fU) | \
    (((A) << 6U) & 0xc0U)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_11_Unk_0],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_11_Unk_0); \
    }


/* static access macros for signal LI0_BCM_11_Unk_1 */

 
#define l_bool_rd_LI0_BCM_11_Unk_1() \
    (LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_11_Unk_1], \
    LIN_BIT_OFFSET_LI0_BCM_11_Unk_1))

#define l_bool_wr_LI0_BCM_11_Unk_1(A) \
    {(A) ? \
    (LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_11_Unk_1], \
    LIN_BIT_OFFSET_LI0_BCM_11_Unk_1)):\
    (LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_11_Unk_1], \
    LIN_BIT_OFFSET_LI0_BCM_11_Unk_1));\
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_11_Unk_1],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_11_Unk_1);}
 
/* static access macros for signal LI0_BCM_11_Unk_2 */
 
#define l_u8_rd_LI0_BCM_11_Unk_2() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_11_Unk_2]) >> 0U) & 0xffU))


#define l_u8_wr_LI0_BCM_11_Unk_2(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_11_Unk_2] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_11_Unk_2] & 0x00U) | \
    (((A) << 0U) & 0xffU)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_11_Unk_2],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_11_Unk_2); \
    }




/* Signal flag APIs */

#define l_flg_tst_LI0_BCM_AutoWipersThreshold_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_AutoWipersThreshold],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_AutoWipersThreshold)
#define l_flg_clr_LI0_BCM_AutoWipersThreshold_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_AutoWipersThreshold],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_AutoWipersThreshold)

#define l_flg_tst_LI0_BCM_AmbientLightOn_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_AmbientLightOn],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_AmbientLightOn)
#define l_flg_clr_LI0_BCM_AmbientLightOn_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_AmbientLightOn],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_AmbientLightOn)

#define l_flg_tst_LI0_RLS_FrontWipersMode_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_FrontWipersMode],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_FrontWipersMode)
#define l_flg_clr_LI0_RLS_FrontWipersMode_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_FrontWipersMode],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_FrontWipersMode)

#define l_flg_tst_LI0_SIG_Error_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_SIG_Error],\
         LIN_FLAG_BIT_OFFSET_LI0_SIG_Error)
#define l_flg_clr_LI0_SIG_Error_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_SIG_Error],\
         LIN_FLAG_BIT_OFFSET_LI0_SIG_Error)

#define l_flg_tst_LI0_RLS_FrontWipersAutoModeEnabled_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_FrontWipersAutoModeEnabled],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_FrontWipersAutoModeEnabled)
#define l_flg_clr_LI0_RLS_FrontWipersAutoModeEnabled_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_FrontWipersAutoModeEnabled],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_FrontWipersAutoModeEnabled)

#define l_flg_tst_LI0_RLS_AutoLightOn_0_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_AutoLightOn_0],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_AutoLightOn_0)
#define l_flg_clr_LI0_RLS_AutoLightOn_0_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_AutoLightOn_0],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_AutoLightOn_0)

#define l_flg_tst_LI0_RLS_AutoLightOn_1_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_AutoLightOn_1],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_AutoLightOn_1)
#define l_flg_clr_LI0_RLS_AutoLightOn_1_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_AutoLightOn_1],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_AutoLightOn_1)

#define l_flg_tst_LI0_RLS_Unknown_1_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_Unknown_1],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_Unknown_1)
#define l_flg_clr_LI0_RLS_Unknown_1_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_Unknown_1],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_Unknown_1)

#define l_flg_tst_LI0_RLS_AmbientLightLevel_0_3_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_AmbientLightLevel_0_3],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_AmbientLightLevel_0_3)
#define l_flg_clr_LI0_RLS_AmbientLightLevel_0_3_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_AmbientLightLevel_0_3],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_AmbientLightLevel_0_3)

#define l_flg_tst_LI0_RLS_Unknown_0_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_Unknown_0],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_Unknown_0)
#define l_flg_clr_LI0_RLS_Unknown_0_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_Unknown_0],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_Unknown_0)

#define l_flg_tst_LI0_RLS_Unknown_2_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_Unknown_2],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_Unknown_2)
#define l_flg_clr_LI0_RLS_Unknown_2_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_Unknown_2],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_Unknown_2)

#define l_flg_tst_LI0_RLS_AmbientLightLevel_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_AmbientLightLevel],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_AmbientLightLevel)
#define l_flg_clr_LI0_RLS_AmbientLightLevel_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_AmbientLightLevel],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_AmbientLightLevel)

#define l_flg_tst_LI0_BCM_WipersSwPos_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_WipersSwPos],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_WipersSwPos)
#define l_flg_clr_LI0_BCM_WipersSwPos_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_WipersSwPos],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_WipersSwPos)

#define l_flg_tst_LI0_BCM_Washer_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_Washer],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_Washer)
#define l_flg_clr_LI0_BCM_Washer_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_Washer],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_Washer)

#define l_flg_tst_LI0_BCM_LightSwitchPos_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_LightSwitchPos],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_LightSwitchPos)
#define l_flg_clr_LI0_BCM_LightSwitchPos_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_LightSwitchPos],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_LightSwitchPos)

#define l_flg_tst_LI0_BCM_Unknown_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_Unknown],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_Unknown)
#define l_flg_clr_LI0_BCM_Unknown_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_Unknown],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_Unknown)

#define l_flg_tst_LI0_BCM_VehicleSpeed_L_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_VehicleSpeed_L],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_VehicleSpeed_L)
#define l_flg_clr_LI0_BCM_VehicleSpeed_L_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_VehicleSpeed_L],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_VehicleSpeed_L)

#define l_flg_tst_LI0_BCM_VehicleSpeed_H_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_VehicleSpeed_H],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_VehicleSpeed_H)
#define l_flg_clr_LI0_BCM_VehicleSpeed_H_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_VehicleSpeed_H],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_VehicleSpeed_H)

#define l_flg_tst_LI0_BCM_BatteryVoltage_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_BatteryVoltage],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_BatteryVoltage)
#define l_flg_clr_LI0_BCM_BatteryVoltage_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_BatteryVoltage],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_BatteryVoltage)

#define l_flg_tst_LI0_RLS_LinError_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_LinError],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_LinError)
#define l_flg_clr_LI0_RLS_LinError_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_LinError],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_LinError)

#define l_flg_tst_LI0_BCM_11_Unk_0_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_11_Unk_0],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_11_Unk_0)
#define l_flg_clr_LI0_BCM_11_Unk_0_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_11_Unk_0],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_11_Unk_0)

#define l_flg_tst_LI0_BCM_11_Unk_1_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_11_Unk_1],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_11_Unk_1)
#define l_flg_clr_LI0_BCM_11_Unk_1_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_11_Unk_1],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_11_Unk_1)

#define l_flg_tst_LI0_BCM_11_Unk_2_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_11_Unk_2],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_11_Unk_2)
#define l_flg_clr_LI0_BCM_11_Unk_2_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_11_Unk_2],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_11_Unk_2)



/* Frame flag APIs */

   /* Interface_name = LI0 */

 #define l_flg_tst_LI0_BCM_55_flag() \
          lin_frame_flag_tbl[LI0_BCM_55]
 #define l_flg_clr_LI0_BCM_55_flag() \
          lin_frame_flag_tbl[LI0_BCM_55] = 0

 #define l_flg_tst_LI0_RLS_8E_flag() \
          lin_frame_flag_tbl[LI0_RLS_8E]
 #define l_flg_clr_LI0_RLS_8E_flag() \
          lin_frame_flag_tbl[LI0_RLS_8E] = 0

 #define l_flg_tst_LI0_BCM_D6_flag() \
          lin_frame_flag_tbl[LI0_BCM_D6]
 #define l_flg_clr_LI0_BCM_D6_flag() \
          lin_frame_flag_tbl[LI0_BCM_D6] = 0

 #define l_flg_tst_LI0_BCM_85_flag() \
          lin_frame_flag_tbl[LI0_BCM_85]
 #define l_flg_clr_LI0_BCM_85_flag() \
          lin_frame_flag_tbl[LI0_BCM_85] = 0

 #define l_flg_tst_LI0_BCM_11_flag() \
          lin_frame_flag_tbl[LI0_BCM_11]
 #define l_flg_clr_LI0_BCM_11_flag() \
          lin_frame_flag_tbl[LI0_BCM_11] = 0

 #define l_flg_tst_LI0_MasterReq_flag() \
          lin_frame_flag_tbl[LI0_MasterReq]
 #define l_flg_clr_LI0_MasterReq_flag() \
          lin_frame_flag_tbl[LI0_MasterReq] = 0

 #define l_flg_tst_LI0_SlaveResp_flag() \
          lin_frame_flag_tbl[LI0_SlaveResp]
 #define l_flg_clr_LI0_SlaveResp_flag() \
          lin_frame_flag_tbl[LI0_SlaveResp] = 0



/* INTERFACE MANAGEMENT */

#define l_ifc_init_LI0() l_ifc_init(LI0)



#define l_ifc_wake_up_LI0() l_ifc_wake_up(LI0)



#define l_ifc_rx_LI0() l_ifc_rx(LI0)



#define l_ifc_tx_LI0() l_ifc_tx(LI0)



#define l_ifc_aux_LI0() l_ifc_aux(LI0)



#define l_ifc_read_status_LI0() l_ifc_read_status(LI0)


#endif    /* _LIN_CFG_H_ */