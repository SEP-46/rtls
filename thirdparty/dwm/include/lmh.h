/*! ------------------------------------------------------------------------------------------------------------------
 * @file    lmh.h
 * @brief   low-level module handshake (LMH) utilities to handle DWM1001 data 
 *          transmission and receiving.
 *          Use LMH_Init() before using to initialize the utilities. 
 *          Use LMH_Tx() to send request message 
 *          Use LMH_WaitForRx() to wait for response message
 *
 * @attention
 *
 * Copyright 2017 (c) Decawave Ltd, Dublin, Ireland.
 *
 * All rights reserved.
 *
 */
#ifndef _LMH_H_
#define _LMH_H_

#include <stdint.h>
#include <stdio.h>
#include "dwm_api.h"

#define LMH_OK    0
#define LMH_ERR   1

#define LMH_Log printf

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*lmh_init_t)(void);
typedef void (*lmh_deinit_t)(void);
typedef int (*lmh_tx_t)(uint8_t* data, uint8_t* length);
typedef int (*lmh_rx_t)(uint8_t* data, uint16_t* length, uint16_t exp_length);

typedef struct {
   lmh_init_t init;
   lmh_deinit_t deinit;
   lmh_tx_t tx;
   lmh_rx_t rx;
} lmh_impl_t;

/** 
 * @brief Sets the implementation of LMH. Must be called before Init.
 *
 * @param[in] p_impl Pointer to structure that contains implementation for TX/RX functions
 *
 * @return none
 */
void LMH_SetImpl(lmh_impl_t* p_impl);

/** 
 * @brief initializes the LMH utilities over defined interface
 *
 * @param none
 *
 * @return none
 */
void LMH_Init(void);


/** 
 * @brief de-initializes the LMH utilities over defined interface
 *
 * @param none
 *
 * @return none
 */
void LMH_DeInit(void);

/** 
 * @brief transmit data over defined interface
 *
 * @param [in] data: pointer to the Tx data buffer
 * @param [in] length: length of data to be received
 *
 * @return Error code
 */
int  LMH_Tx(uint8_t* data, uint8_t* length);

/** 
 * @brief wait for response data over defined interface
 *       note: this function is blocking 
 *
 * @param [out] data: pointer to the RX data buffer
 * @param [out] length: length of data to be received
 * @param [in] exp_length: expected length of response data. 
 *             Note - If the user doesn't know how long the response from DWM1001 to the host 
 *                   is, then this parameter should be set to DWM1001_TLV_MAX_SIZE as defined
 *                   in dwm1001.h. In this case,
 *                   for SPI, length check won't report error no matter how long the received
 *                   data is;
 *                   for UART, this function will not return until the timeout period expires. 
 *
 * @return Error code
 */
int  LMH_WaitForRx(uint8_t* data, uint16_t* length, uint16_t exp_length);

/**
 * @brief check if GPIO pin: index is among available pins
 *
 * @param[in] idx, GPIO pin index
 *
 * @return Error code
 */
static inline int LMH_CheckGPIOIdx(dwm_gpio_idx_t idx)
{
   if((idx == 2 ) || (idx == 8 ) || (idx == 9 ) || (idx == 10) || (idx == 12)  \
   || (idx == 13) || (idx == 14) || (idx == 15) || (idx == 22) || (idx == 23)  \
   || (idx == 27) || (idx == 30) || (idx == 31))
   {
      return LMH_OK; //good
   } 
   else
   {
      return LMH_ERR; //error
   }
}

/** 
 * @brief wait for response data over defined interface
 *       note: this function is blocking 
 *
 * @param [in] ret_val: pointer to the response data buffer, where the first three bytes must 
 *       be TLV values 0x40, 0x01, 0x00 meaning a RV_OK, to indicating that the request is 
 *       properly parsed. Otherwise the previous communication between the host and DWM1001 
 *       was not acting correctly.
 *
 * @return Error code
 */
static inline int LMH_CheckRetVal(uint8_t* ret_val)
{
   if(ret_val[0] != DWM1001_TLV_TYPE_RET_VAL)
   {
      LMH_Log("lmh: *** ERROR ***: RET_VAL type wrong: %d\n", ret_val[0]);    
      return LMH_ERR;
   }
   if(ret_val[1] != 1)
   {
      LMH_Log("lmh: *** ERROR ***: RET_VAL length wrong: %d\n", ret_val[1]);    
      return LMH_ERR;
   }
   
   if(ret_val[2] == RV_OK)
   { 
      return LMH_OK;
   }   
   else
   {
      LMH_Log("lmh: *** ERROR ***: DWM1001_RV_ERR: %d\n", ret_val[2]);    
      return LMH_ERR;
   }
}

#ifdef __cplusplus
}
#endif

#endif //_LMH_H_

