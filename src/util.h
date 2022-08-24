#pragma once

#include <cstdint>

using Timestamp_t = int64_t;

// Gets number of milliseconds that have elapsed since program start
Timestamp_t Util_GetCurrentTime();