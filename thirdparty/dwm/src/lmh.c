#include "lmh.h"

#include <stddef.h>

//
// LMH is the Low-level Module Handshake API, it allows communicating with the DWM1001
// over an abstract interface. In the original examples, this was implemented using
// the Raspberry Pi HAL API, which isn't very portable.
// 
// This file instead implements the LMH interface dynamically by exposing them as function
// pointers that the user sets
//

static lmh_impl_t* g_impl;

void LMH_SetImpl(lmh_impl_t* p_impl)
{
   g_impl = p_impl;
}

void LMH_Init(void)
{
   g_impl->init();
}

void LMH_DeInit(void)
{
   g_impl->deinit();
   g_impl = NULL;
}

int  LMH_Tx(uint8_t* data, uint8_t* length)
{
   return g_impl->tx(data, length);
}

int  LMH_WaitForRx(uint8_t* data, uint16_t* length, uint16_t exp_length)
{
   return g_impl->rx(data, length, exp_length);
}