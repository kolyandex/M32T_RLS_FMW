/******************************************************************************
* 
* Freescale Semiconductor Inc.
* (c) Copyright 2013-2016 Freescale Semiconductor, Inc.
* Copyright 2016-2025 NXP
* ALL RIGHTS RESERVED.
* 
****************************************************************************//*!
*
* @file      lin_cfg.c
*
* @author    FPT Software
*
* @version   1.0
*
* @date      Tue Sep 09 23:30:50 NOVT 2025
*
* @brief     Common LIN configuration, data structure
*
******************************************************************************/
#include "lin_cfg.h"
#include "lin.h"
/* Mapping interface with hardware */
const lin_hardware_name lin_virtual_ifc = LINUART0;
l_u8 lin_lld_response_buffer[10];
l_u8 lin_successful_transfer;
l_u8 lin_error_in_response;
l_u8 lin_goto_sleep_flg;
/* Save configuration flag */
l_u8 lin_save_configuration_flg = 0;
lin_word_status_str lin_word_status;
l_u8 lin_current_pid;

const l_signal_handle LI0_response_error_signal = LI0_SIG_Error;

volatile l_u8 buffer_backup_data[8];

/* definition and initialization of signal array */
l_u8    lin_pFrameBuf[LIN_FRAME_BUF_SIZE] =
{


  0x00 /* 0 : 00000000 */ /* start of frame LI0_BCM_55 */

  ,0xf0 /* 1 : 11110000 */


  ,0x84 /* 2 : 10000100 */ /* start of frame LI0_RLS_8E */

  ,0x86 /* 3 : 10000110 */
  
  ,0x80 /* 4 : 10000000 */
  
  ,0xff /* 5 : 11111111 */
  
  ,0xff /* 6 : 11111111 */
  
  ,0xff /* 7 : 11111111 */
  
  ,0xff /* 8 : 11111111 */
  
  ,0xff /* 9 : 11111111 */
  

  ,0xdf /* 10 : 11011111 */ /* start of frame LI0_BCM_D6 */

  ,0xff /* 11 : 11111111 */
  
  ,0xff /* 12 : 11111111 */
  
  ,0xff /* 13 : 11111111 */
  
  ,0xff /* 14 : 11111111 */
  
  ,0xff /* 15 : 11111111 */
  
  ,0xff /* 16 : 11111111 */
  
  ,0xff /* 17 : 11111111 */
  

  ,0xff /* 18 : 11111111 */ /* start of frame LI0_BCM_85 */

  ,0xff /* 19 : 11111111 */
  
  ,0xff /* 20 : 11111111 */
  
  ,0xf8 /* 21 : 11111000 */
  
  ,0x0f /* 22 : 00001111 */
  
  ,0x00 /* 23 : 00000000 */
  
  ,0xff /* 24 : 11111111 */
  
  ,0x00 /* 25 : 00000000 */
  

  ,0xff /* 26 : 11111111 */ /* start of frame LI0_BCM_11 */

  ,0xff /* 27 : 11111111 */
  
  ,0x00 /* 28 : 00000000 */
  
  ,0xff /* 29 : 11111111 */
  
  ,0xf8 /* 30 : 11111000 */
  
  ,0xff /* 31 : 11111111 */
  
  ,0xff /* 32 : 11111111 */
  
  ,0xff /* 33 : 11111111 */
  
};

/* definition and initialization of signal array */
l_u8    lin_flag_handle_tbl[LIN_FLAG_BUF_SIZE] =
{


  0xFF /* 0: start of flag frame LI0_BCM_55 */



  ,0xFF /* 1: start of flag frame LI0_RLS_8E */

  ,0xFF /* 2: */
  

  ,0xFF /* 3: start of flag frame LI0_BCM_D6 */


  ,0xFF /* 4: start of flag frame LI0_BCM_85 */


  ,0xFF /* 5: start of flag frame LI0_BCM_11 */

};

/*************************** Flag set when signal is updated ******************/
/* Diagnostic signal */
l_u8 lin_diag_signal_tbl[16];
/*****************************event trigger frame*****************************/

/**********************************  Frame table **********************************/
const lin_frame_struct lin_frame_tbl[LIN_NUM_OF_FRMS] ={

    { LIN_FRM_UNCD, 2, LIN_RES_SUB, 0, 0, 1   , (l_u8*)0 }

   ,{ LIN_FRM_UNCD, 8, LIN_RES_PUB, 2, 1, 2 , (l_u8*)&LI0_response_error_signal }
  
   ,{ LIN_FRM_UNCD, 8, LIN_RES_SUB, 10, 3, 1 , (l_u8*)0 }
  
   ,{ LIN_FRM_UNCD, 8, LIN_RES_SUB, 18, 4, 1 , (l_u8*)0 }
  
   ,{ LIN_FRM_UNCD, 8, LIN_RES_SUB, 26, 5, 1 , (l_u8*)0 }
  
   ,{ LIN_FRM_DIAG, 8, LIN_RES_SUB, 0, 0, 0 , (l_u8*)0 }
  
   ,{ LIN_FRM_DIAG, 8, LIN_RES_PUB, 0, 0, 0 , (l_u8*)0 }
  
};

/*********************************** Frame flag Initialization **********************/
/*************************** Frame flag for send/receive successfully ***************/
l_bool lin_frame_flag_tbl[LIN_NUM_OF_FRMS] = {0, 0, 0, 0, 0, 0, 0};
/*************************** Frame flag for updating signal in frame ****************/
volatile l_u8 lin_frame_updating_flag_tbl[LIN_NUM_OF_FRMS] = {0, 0, 0, 0, 0, 0, 0};


/**************************** Lin configuration Initialization ***********************/
/* max_response_frame_timeout = round((1.4x(10+Nx10)xTbit)/Tbase_period) + 3 */

const l_u16 lin_max_frame_res_timeout_val[8]={

6, 7, 9, 10, 12, 13, 15, 16

};


l_u8 lin_configuration_RAM[LIN_SIZE_OF_CFG]= {0x00, 0x15, 0x0E, 0x16, 0x05, 0x11, 0x3C, 0x3D ,0xFF};


const l_u16  lin_configuration_ROM[LIN_SIZE_OF_CFG]= {0x00, 0x15, 0x0E, 0x16, 0x05, 0x11, 0x3C, 0x3D ,0xFFFF};

/***************************************** Node Attribute*****************************************/

l_u8 lin_configured_NAD = 0x01;    /*<configured_NAD>*/
const l_u8 lin_initial_NAD    =0x01;    /*<initial_NAD>*/
const lin_product_id product_id = {0x00FA, 0x0001, 0x0000 };  /* {<supplier_id>,<function_id>,<variant>} */
const l_signal_handle response_error =  LI0_SIG_Error;
const l_u8 num_frame_have_esignal = 1;                                 /*number of frame contain error signal*/
const l_u16 lin_response_error_byte_offset[1] = {LIN_BYTE_OFFSET_LI0_SIG_Error};                  /*<interface_name>_< response_error>*/
const l_u8 lin_response_error_bit_offset[1] = {LIN_BIT_OFFSET_LI0_SIG_Error};                  /*<interface_name>_< response_error>*/

/************************** TL Layer and Diagnostic: SINGLE interface **************************/
/* QUEUE information */
lin_tl_pdu_data tl_tx_queue_data[MAX_QUEUE_SIZE];    /*transmit queue data */
lin_tl_pdu_data tl_rx_queue_data[MAX_QUEUE_SIZE];    /*receive queue data */

lin_transport_layer_queue lin_tl_tx_queue = {
0,                                                /* the first element of queue */
0,                                                /* the last element of queue */
LD_QUEUE_EMPTY,                                   /* status of queue */
0,                                                /* curernt size of queue */
MAX_QUEUE_SIZE,                                   /* size of queue */
tl_tx_queue_data,                                 /* data of queue */
};
lin_transport_layer_queue lin_tl_rx_queue = {
0,                                                /* the first element of queue */
0,                                                /* the last element of queue */
LD_QUEUE_EMPTY,                                   /* status of queue */
0,                                                /* curernt size of queue */
MAX_QUEUE_SIZE,                                   /* size of queue */
tl_rx_queue_data,                                 /* data of queue */
};
/* message information in transmit queue */
l_u16 tl_rx_msg_index;                                /* index of message in queue */
l_u16 tl_rx_msg_size;                                 /* Size of message in queue */
/* message information in receive queue */
l_u16 tl_tx_msg_index;                                /* index of message in queue */
l_u16 tl_tx_msg_size;                                 /* Size of message in queue */
lin_last_cfg_result tl_last_cfg_result;               /* Status of the last configuration service in LIN 2.0, J2602 */
l_u8 tl_last_RSID;                                    /* RSID of the last node configuration service */
l_u8 tl_ld_error_code;                                /* Error code in case of positive response */
l_u8 tl_no_of_pdu;                                    /* number of received pdu */
l_u8 tl_frame_counter;                                /* frame counter in received message */
lin_message_timeout_type tl_check_timeout_type;       /* timeout type */
l_u16 tl_check_timeout;                               /* timeout counter*/
l_u8 *tl_ident_data;                                  /* To store address of RAM area contain response */

lin_diagnostic_state tl_diag_state = LD_DIAG_IDLE;
lin_service_status tl_service_status =  LD_SERVICE_IDLE ; /* service status */
lin_message_status tl_receive_msg_status;             /* receive message status */
lin_message_status tl_rx_msg_status;                  /* cooked rx status */
lin_message_status tl_tx_msg_status;                  /* cooked tx status */











/****************************Support SID Initialization ***********************/

const l_u8 lin_diag_services_supported[_DIAG_NUMBER_OF_SERVICES_] = {0x10,0x22,0x2E,0x31};
l_u8 lin_diag_services_flag[_DIAG_NUMBER_OF_SERVICES_] = {0,0,0,0};

l_u8 tl_slaveresp_cnt = 0;
/*This ld_read_by_id_callout() function is used when the master node transmits a read by
 identifier request with an identifier in the user defined area (id from 32 to 63).
 The driver will call this function when such request is received.
 * id: the identifier in the user defined area (32 to 63)
 * data: pointer points to a data area with 5 bytes, used to give the positive response.
  Driver uses 0xFF "do not care value" for unassigned data values.
  Data length in PCI is (1 + number of assigned meaningful data values).
  Driver will take as data for all data before and including the last value in the frame that is different from 0xFF.
  PCI is 0x02-0x06, so data should have at least one value different from 0xFF.
  For example, a response frame, (NAD) (PCI) (0xF2) (0xFF) (0x00) (0xFF) (0xFF) (0xFF),
  PCI will be 0x03, since in this case driver takes all data before 0x00 and 0x00 as meaningful data,
  and values after 0x00 are do not care value.
 * return: LD_NEGATIVE_RESPONSE Respond with a negative response.
           LD_POSTIVE_RESPONSE Respond with a positive response.
           LD_ID_NO_RESPONSE The slave node will not answer.
 */
l_u8 ld_read_by_id_callout(l_u8 id, l_u8 *data)
{
    l_u8 retval = LD_NEGATIVE_RESPONSE;
    /* Following code is an example - Real implementation is application-dependent */
    /* This example implement with ID = 32 - LIN_READ_USR_DEF_MIN */
    if (id == LIN_READ_USR_DEF_MIN)
    {
      /* id received is user defined 32 */
      data[0] = (l_u8) (id + 1);    /* Data user define */
      data[1] = (l_u8) (id + 2);    /* Data user define */
      data[2] = (l_u8) (id + 3);    /* Data user define */
      data[3] = (l_u8) (id + 4);    /* Data user define */
      data[4] = (l_u8) (id + 5);    /* Data user define */
      retval = LD_POSITIVE_RESPONSE;
    }
    else
    {
      /* other identifiers, respond with negative response by default*/
    }
    return retval;
}