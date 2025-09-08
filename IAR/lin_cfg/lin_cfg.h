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
* @date      Mon Sep 08 23:21:59 NOVT 2025
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


#define _DIAG_NUMBER_OF_SERVICES_    4

#define DIAGSRV_DIAG_SESSION_CONTROL_ORDER    0

#define DIAGSRV_READ_DATA_BY_IDENTIFIER_ORDER    1

#define DIAGSRV_WRITE_DATA_BY_IDENTIFIER_ORDER    2

#define DIAGSRV_IO_CONTROL_ORDER    3


/**************** FRAME SUPPORT DEFINITION ******************/
#define _TL_SINGLE_FRAME_       0
#define _TL_MULTI_FRAME_        1

#define _TL_FRAME_SUPPORT_      _TL_MULTI_FRAME_

/* frame buffer size */
#define LIN_FRAME_BUF_SIZE          34
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
#define LIN_NUM_OF_SIGS  26
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
  
   , LI0_RLS_AmbientLightLevel_pow
  
   , LI0_RLS_AmbientLightDiffRelated
  
   , LI0_RLS_RainDetectedCloseWindows
  
   , LI0_RLS_AmbientLightLevel
  
   , LI0_BCM_WipersSwPos
  
   , LI0_BCM_Washer
  
   , LI0_BCM_LightSwitchPos
  
   , LI0_BCM_WipersInOperationNow
  
   , LI0_BCM_VehicleSpeed
  
   , LI0_BCM_BatteryVoltage
  
   , LI0_RLS_LinError
  
   , LI0_BCM_11_Unk_0
  
   , LI0_BCM_11_Unk_1
  
   , LI0_BCM_11_Unk_2
  
   , LI0_RLS_8E_Alw_1
  
   , LI0_RLS_8E_Alw_1_0
  
   , LI0_RLS_8E_Alw_0
  
   , LI0_BCM_85_IgnState
  
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
#define LIN_BIT_OFFSET_LI0_BCM_AutoWipersThreshold    5U
#define LIN_SIGNAL_SIZE_LI0_BCM_AutoWipersThreshold    3U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_AutoWipersThreshold    0U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_AutoWipersThreshold    2U


#define LIN_BYTE_OFFSET_LI0_BCM_AmbientLightOn    10U
#define LIN_BIT_OFFSET_LI0_BCM_AmbientLightOn    5U
#define LIN_SIGNAL_SIZE_LI0_BCM_AmbientLightOn    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_AmbientLightOn    3U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_AmbientLightOn    0U


#define LIN_BYTE_OFFSET_LI0_RLS_FrontWipersMode    2U
#define LIN_BIT_OFFSET_LI0_RLS_FrontWipersMode    0U
#define LIN_SIGNAL_SIZE_LI0_RLS_FrontWipersMode    2U
#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_FrontWipersMode    1U
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_FrontWipersMode    0U


#define LIN_BYTE_OFFSET_LI0_SIG_Error    2U
#define LIN_BIT_OFFSET_LI0_SIG_Error    3U
#define LIN_SIGNAL_SIZE_LI0_SIG_Error    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_SIG_Error    1U
#define LIN_FLAG_BIT_OFFSET_LI0_SIG_Error    2U


#define LIN_BYTE_OFFSET_LI0_RLS_FrontWipersAutoModeEnabled    2U
#define LIN_BIT_OFFSET_LI0_RLS_FrontWipersAutoModeEnabled    2U
#define LIN_SIGNAL_SIZE_LI0_RLS_FrontWipersAutoModeEnabled    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_FrontWipersAutoModeEnabled    1U
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_FrontWipersAutoModeEnabled    1U


#define LIN_BYTE_OFFSET_LI0_RLS_AutoLightOn_0    2U
#define LIN_BIT_OFFSET_LI0_RLS_AutoLightOn_0    5U
#define LIN_SIGNAL_SIZE_LI0_RLS_AutoLightOn_0    2U
#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_AutoLightOn_0    1U
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_AutoLightOn_0    4U


#define LIN_BYTE_OFFSET_LI0_RLS_AutoLightOn_1    2U
#define LIN_BIT_OFFSET_LI0_RLS_AutoLightOn_1    4U
#define LIN_SIGNAL_SIZE_LI0_RLS_AutoLightOn_1    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_AutoLightOn_1    1U
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_AutoLightOn_1    3U


#define LIN_BYTE_OFFSET_LI0_RLS_Unknown_1    2U
#define LIN_BIT_OFFSET_LI0_RLS_Unknown_1    7U
#define LIN_SIGNAL_SIZE_LI0_RLS_Unknown_1    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_Unknown_1    1U
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_Unknown_1    5U


#define LIN_BYTE_OFFSET_LI0_RLS_AmbientLightLevel_pow    3U
#define LIN_BIT_OFFSET_LI0_RLS_AmbientLightLevel_pow    4U
#define LIN_SIGNAL_SIZE_LI0_RLS_AmbientLightLevel_pow    2U
#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_AmbientLightLevel_pow    2U
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_AmbientLightLevel_pow    1U


#define LIN_BYTE_OFFSET_LI0_RLS_AmbientLightDiffRelated    3U
#define LIN_BIT_OFFSET_LI0_RLS_AmbientLightDiffRelated    1U
#define LIN_SIGNAL_SIZE_LI0_RLS_AmbientLightDiffRelated    2U
#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_AmbientLightDiffRelated    1U
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_AmbientLightDiffRelated    7U


#define LIN_BYTE_OFFSET_LI0_RLS_RainDetectedCloseWindows    3U
#define LIN_BIT_OFFSET_LI0_RLS_RainDetectedCloseWindows    6U
#define LIN_SIGNAL_SIZE_LI0_RLS_RainDetectedCloseWindows    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_RainDetectedCloseWindows    2U
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_RainDetectedCloseWindows    2U


#define LIN_BYTE_OFFSET_LI0_RLS_AmbientLightLevel    4U
#define LIN_BIT_OFFSET_LI0_RLS_AmbientLightLevel    0U
#define LIN_SIGNAL_SIZE_LI0_RLS_AmbientLightLevel    7U
#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_AmbientLightLevel    2U
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_AmbientLightLevel    4U


#define LIN_BYTE_OFFSET_LI0_BCM_WipersSwPos    0U
#define LIN_BIT_OFFSET_LI0_BCM_WipersSwPos    0U
#define LIN_SIGNAL_SIZE_LI0_BCM_WipersSwPos    3U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_WipersSwPos    0U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_WipersSwPos    0U


#define LIN_BYTE_OFFSET_LI0_BCM_Washer    0U
#define LIN_BIT_OFFSET_LI0_BCM_Washer    3U
#define LIN_SIGNAL_SIZE_LI0_BCM_Washer    2U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_Washer    0U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_Washer    1U


#define LIN_BYTE_OFFSET_LI0_BCM_LightSwitchPos    1U
#define LIN_BIT_OFFSET_LI0_BCM_LightSwitchPos    0U
#define LIN_SIGNAL_SIZE_LI0_BCM_LightSwitchPos    3U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_LightSwitchPos    0U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_LightSwitchPos    3U


#define LIN_BYTE_OFFSET_LI0_BCM_WipersInOperationNow    1U
#define LIN_BIT_OFFSET_LI0_BCM_WipersInOperationNow    3U
#define LIN_SIGNAL_SIZE_LI0_BCM_WipersInOperationNow    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_WipersInOperationNow    0U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_WipersInOperationNow    4U


#define LIN_BYTE_OFFSET_LI0_BCM_VehicleSpeed    22U
#define LIN_BIT_OFFSET_LI0_BCM_VehicleSpeed    4U
#define LIN_SIGNAL_SIZE_LI0_BCM_VehicleSpeed    12U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_VehicleSpeed    4U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_VehicleSpeed    1U


#define LIN_BYTE_OFFSET_LI0_BCM_BatteryVoltage    25U
#define LIN_BIT_OFFSET_LI0_BCM_BatteryVoltage    0U
#define LIN_SIGNAL_SIZE_LI0_BCM_BatteryVoltage    8U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_BatteryVoltage    4U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_BatteryVoltage    2U


#define LIN_BYTE_OFFSET_LI0_RLS_LinError    3U
#define LIN_BIT_OFFSET_LI0_RLS_LinError    3U
#define LIN_SIGNAL_SIZE_LI0_RLS_LinError    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_LinError    2U
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_LinError    0U


#define LIN_BYTE_OFFSET_LI0_BCM_11_Unk_0    30U
#define LIN_BIT_OFFSET_LI0_BCM_11_Unk_0    0U
#define LIN_SIGNAL_SIZE_LI0_BCM_11_Unk_0    2U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_11_Unk_0    5U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_11_Unk_0    1U


#define LIN_BYTE_OFFSET_LI0_BCM_11_Unk_1    30U
#define LIN_BIT_OFFSET_LI0_BCM_11_Unk_1    2U
#define LIN_SIGNAL_SIZE_LI0_BCM_11_Unk_1    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_11_Unk_1    5U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_11_Unk_1    2U


#define LIN_BYTE_OFFSET_LI0_BCM_11_Unk_2    28U
#define LIN_BIT_OFFSET_LI0_BCM_11_Unk_2    0U
#define LIN_SIGNAL_SIZE_LI0_BCM_11_Unk_2    8U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_11_Unk_2    5U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_11_Unk_2    0U


#define LIN_BYTE_OFFSET_LI0_RLS_8E_Alw_1    3U
#define LIN_BIT_OFFSET_LI0_RLS_8E_Alw_1    7U
#define LIN_SIGNAL_SIZE_LI0_RLS_8E_Alw_1    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_8E_Alw_1    2U
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_8E_Alw_1    3U


#define LIN_BYTE_OFFSET_LI0_RLS_8E_Alw_1_0    4U
#define LIN_BIT_OFFSET_LI0_RLS_8E_Alw_1_0    7U
#define LIN_SIGNAL_SIZE_LI0_RLS_8E_Alw_1_0    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_8E_Alw_1_0    2U
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_8E_Alw_1_0    5U


#define LIN_BYTE_OFFSET_LI0_RLS_8E_Alw_0    3U
#define LIN_BIT_OFFSET_LI0_RLS_8E_Alw_0    0U
#define LIN_SIGNAL_SIZE_LI0_RLS_8E_Alw_0    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_RLS_8E_Alw_0    1U
#define LIN_FLAG_BIT_OFFSET_LI0_RLS_8E_Alw_0    6U


#define LIN_BYTE_OFFSET_LI0_BCM_85_IgnState    21U
#define LIN_BIT_OFFSET_LI0_BCM_85_IgnState    0U
#define LIN_SIGNAL_SIZE_LI0_BCM_85_IgnState    2U
#define LIN_FLAG_BYTE_OFFSET_LI0_BCM_85_IgnState    4U
#define LIN_FLAG_BIT_OFFSET_LI0_BCM_85_IgnState    0U




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
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_AutoWipersThreshold]) >> 5U) & 0x07U))


#define l_u8_wr_LI0_BCM_AutoWipersThreshold(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_AutoWipersThreshold] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_AutoWipersThreshold] & 0x1fU) | \
    (((A) << 5U) & 0xe0U)); \
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
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_FrontWipersMode]) >> 0U) & 0x03U))


#define l_u8_wr_LI0_RLS_FrontWipersMode(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_FrontWipersMode] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_FrontWipersMode] & 0xfcU) | \
    (((A) << 0U) & 0x03U)); \
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
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AutoLightOn_0]) >> 5U) & 0x03U))


#define l_u8_wr_LI0_RLS_AutoLightOn_0(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AutoLightOn_0] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AutoLightOn_0] & 0x9fU) | \
    (((A) << 5U) & 0x60U)); \
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
 
/* static access macros for signal LI0_RLS_AmbientLightLevel_pow */
 
#define l_u8_rd_LI0_RLS_AmbientLightLevel_pow() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AmbientLightLevel_pow]) >> 4U) & 0x03U))


#define l_u8_wr_LI0_RLS_AmbientLightLevel_pow(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AmbientLightLevel_pow] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AmbientLightLevel_pow] & 0xcfU) | \
    (((A) << 4U) & 0x30U)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_AmbientLightLevel_pow],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_AmbientLightLevel_pow); \
    }


 
/* static access macros for signal LI0_RLS_AmbientLightDiffRelated */
 
#define l_u8_rd_LI0_RLS_AmbientLightDiffRelated() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AmbientLightDiffRelated]) >> 1U) & 0x03U))


#define l_u8_wr_LI0_RLS_AmbientLightDiffRelated(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AmbientLightDiffRelated] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AmbientLightDiffRelated] & 0xf9U) | \
    (((A) << 1U) & 0x06U)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_AmbientLightDiffRelated],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_AmbientLightDiffRelated); \
    }


/* static access macros for signal LI0_RLS_RainDetectedCloseWindows */

 
#define l_bool_rd_LI0_RLS_RainDetectedCloseWindows() \
    (LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_RainDetectedCloseWindows], \
    LIN_BIT_OFFSET_LI0_RLS_RainDetectedCloseWindows))

#define l_bool_wr_LI0_RLS_RainDetectedCloseWindows(A) \
    {(A) ? \
    (LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_RainDetectedCloseWindows], \
    LIN_BIT_OFFSET_LI0_RLS_RainDetectedCloseWindows)):\
    (LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_RainDetectedCloseWindows], \
    LIN_BIT_OFFSET_LI0_RLS_RainDetectedCloseWindows));\
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_RainDetectedCloseWindows],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_RainDetectedCloseWindows);}
 
/* static access macros for signal LI0_RLS_AmbientLightLevel */
 
#define l_u8_rd_LI0_RLS_AmbientLightLevel() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AmbientLightLevel]) >> 0U) & 0x7fU))


#define l_u8_wr_LI0_RLS_AmbientLightLevel(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AmbientLightLevel] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_AmbientLightLevel] & 0x80U) | \
    (((A) << 0U) & 0x7fU)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_AmbientLightLevel],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_AmbientLightLevel); \
    }


 
/* static access macros for signal LI0_BCM_WipersSwPos */
 
#define l_u8_rd_LI0_BCM_WipersSwPos() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_WipersSwPos]) >> 0U) & 0x07U))


#define l_u8_wr_LI0_BCM_WipersSwPos(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_WipersSwPos] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_WipersSwPos] & 0xf8U) | \
    (((A) << 0U) & 0x07U)); \
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
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_LightSwitchPos]) >> 0U) & 0x07U))


#define l_u8_wr_LI0_BCM_LightSwitchPos(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_LightSwitchPos] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_LightSwitchPos] & 0xf8U) | \
    (((A) << 0U) & 0x07U)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_LightSwitchPos],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_LightSwitchPos); \
    }


/* static access macros for signal LI0_BCM_WipersInOperationNow */

 
#define l_bool_rd_LI0_BCM_WipersInOperationNow() \
    (LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_WipersInOperationNow], \
    LIN_BIT_OFFSET_LI0_BCM_WipersInOperationNow))

#define l_bool_wr_LI0_BCM_WipersInOperationNow(A) \
    {(A) ? \
    (LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_WipersInOperationNow], \
    LIN_BIT_OFFSET_LI0_BCM_WipersInOperationNow)):\
    (LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_WipersInOperationNow], \
    LIN_BIT_OFFSET_LI0_BCM_WipersInOperationNow));\
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_WipersInOperationNow],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_WipersInOperationNow);}
 
/* static access macros for signal LI0_BCM_VehicleSpeed */
 
#define l_u16_rd_LI0_BCM_VehicleSpeed() \
    ((l_u16)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_VehicleSpeed] + (lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_VehicleSpeed + 1U] << 8U)) >> 4U) & 0xfffU))


#define l_u16_wr_LI0_BCM_VehicleSpeed(A) \
    { \
    buffer_backup_data[4U] =  lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_VehicleSpeed]; \
    lin_frame_updating_flag_tbl[LI0_BCM_85] |= (1U << 4); \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_VehicleSpeed] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_VehicleSpeed] & 0x0fU) | \
    (((A) << 4U) & 0xf0U)); \
    buffer_backup_data[4U + 1U] =  lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_VehicleSpeed + 1U]; \
    lin_frame_updating_flag_tbl[LI0_BCM_85] |= (1U << (4 + 1U)); \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_VehicleSpeed + 1U] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_VehicleSpeed + 1U] & 0x00U) | \
    (((A) >> 4U) & 0xffU)); \
    lin_frame_updating_flag_tbl[LI0_BCM_85] &= (~(0x03 << 4)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_VehicleSpeed],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_VehicleSpeed); \
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
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_11_Unk_0]) >> 0U) & 0x03U))


#define l_u8_wr_LI0_BCM_11_Unk_0(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_11_Unk_0] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_11_Unk_0] & 0xfcU) | \
    (((A) << 0U) & 0x03U)); \
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


/* static access macros for signal LI0_RLS_8E_Alw_1 */

 
#define l_bool_rd_LI0_RLS_8E_Alw_1() \
    (LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_8E_Alw_1], \
    LIN_BIT_OFFSET_LI0_RLS_8E_Alw_1))

#define l_bool_wr_LI0_RLS_8E_Alw_1(A) \
    {(A) ? \
    (LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_8E_Alw_1], \
    LIN_BIT_OFFSET_LI0_RLS_8E_Alw_1)):\
    (LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_8E_Alw_1], \
    LIN_BIT_OFFSET_LI0_RLS_8E_Alw_1));\
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_8E_Alw_1],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_8E_Alw_1);}
/* static access macros for signal LI0_RLS_8E_Alw_1_0 */

 
#define l_bool_rd_LI0_RLS_8E_Alw_1_0() \
    (LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_8E_Alw_1_0], \
    LIN_BIT_OFFSET_LI0_RLS_8E_Alw_1_0))

#define l_bool_wr_LI0_RLS_8E_Alw_1_0(A) \
    {(A) ? \
    (LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_8E_Alw_1_0], \
    LIN_BIT_OFFSET_LI0_RLS_8E_Alw_1_0)):\
    (LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_8E_Alw_1_0], \
    LIN_BIT_OFFSET_LI0_RLS_8E_Alw_1_0));\
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_8E_Alw_1_0],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_8E_Alw_1_0);}
/* static access macros for signal LI0_RLS_8E_Alw_0 */

 
#define l_bool_rd_LI0_RLS_8E_Alw_0() \
    (LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_8E_Alw_0], \
    LIN_BIT_OFFSET_LI0_RLS_8E_Alw_0))

#define l_bool_wr_LI0_RLS_8E_Alw_0(A) \
    {(A) ? \
    (LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_8E_Alw_0], \
    LIN_BIT_OFFSET_LI0_RLS_8E_Alw_0)):\
    (LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_RLS_8E_Alw_0], \
    LIN_BIT_OFFSET_LI0_RLS_8E_Alw_0));\
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_8E_Alw_0],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_8E_Alw_0);}
 
/* static access macros for signal LI0_BCM_85_IgnState */
 
#define l_u8_rd_LI0_BCM_85_IgnState() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_85_IgnState]) >> 0U) & 0x03U))


#define l_u8_wr_LI0_BCM_85_IgnState(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_85_IgnState] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_BCM_85_IgnState] & 0xfcU) | \
    (((A) << 0U) & 0x03U)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_85_IgnState],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_85_IgnState); \
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

#define l_flg_tst_LI0_RLS_AmbientLightLevel_pow_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_AmbientLightLevel_pow],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_AmbientLightLevel_pow)
#define l_flg_clr_LI0_RLS_AmbientLightLevel_pow_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_AmbientLightLevel_pow],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_AmbientLightLevel_pow)

#define l_flg_tst_LI0_RLS_AmbientLightDiffRelated_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_AmbientLightDiffRelated],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_AmbientLightDiffRelated)
#define l_flg_clr_LI0_RLS_AmbientLightDiffRelated_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_AmbientLightDiffRelated],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_AmbientLightDiffRelated)

#define l_flg_tst_LI0_RLS_RainDetectedCloseWindows_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_RainDetectedCloseWindows],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_RainDetectedCloseWindows)
#define l_flg_clr_LI0_RLS_RainDetectedCloseWindows_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_RainDetectedCloseWindows],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_RainDetectedCloseWindows)

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

#define l_flg_tst_LI0_BCM_WipersInOperationNow_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_WipersInOperationNow],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_WipersInOperationNow)
#define l_flg_clr_LI0_BCM_WipersInOperationNow_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_WipersInOperationNow],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_WipersInOperationNow)

#define l_flg_tst_LI0_BCM_VehicleSpeed_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_VehicleSpeed],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_VehicleSpeed)
#define l_flg_clr_LI0_BCM_VehicleSpeed_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_VehicleSpeed],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_VehicleSpeed)

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

#define l_flg_tst_LI0_RLS_8E_Alw_1_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_8E_Alw_1],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_8E_Alw_1)
#define l_flg_clr_LI0_RLS_8E_Alw_1_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_8E_Alw_1],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_8E_Alw_1)

#define l_flg_tst_LI0_RLS_8E_Alw_1_0_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_8E_Alw_1_0],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_8E_Alw_1_0)
#define l_flg_clr_LI0_RLS_8E_Alw_1_0_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_8E_Alw_1_0],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_8E_Alw_1_0)

#define l_flg_tst_LI0_RLS_8E_Alw_0_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_8E_Alw_0],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_8E_Alw_0)
#define l_flg_clr_LI0_RLS_8E_Alw_0_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_RLS_8E_Alw_0],\
         LIN_FLAG_BIT_OFFSET_LI0_RLS_8E_Alw_0)

#define l_flg_tst_LI0_BCM_85_IgnState_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_85_IgnState],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_85_IgnState)
#define l_flg_clr_LI0_BCM_85_IgnState_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_BCM_85_IgnState],\
         LIN_FLAG_BIT_OFFSET_LI0_BCM_85_IgnState)



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