#include "util.h"

#include <chrono>

static auto start = std::chrono::high_resolution_clock::now();

int64_t Util_GetCurrentTime()
{
	auto now = std::chrono::high_resolution_clock::now();
	int64_t elapsed = std::chrono::duration_cast<std::chrono::milliseconds>( now - start ).count();

	return elapsed;
}