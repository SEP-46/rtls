#pragma once

//
// LMH is the Low-level Module Handshake API, it allows communicating with the DWM1001
// over an abstract interface. In the original examples, this was implemented using
// the Raspberry Pi HAL API, which isn't very portable.
// 
// This file instead implements the LMH interface by using the serial library, which
// works on various different platforms, including Windows/Linux.
//

#include <lmh.h>

extern lmh_impl_t* LMH_Serial_GetImpl();