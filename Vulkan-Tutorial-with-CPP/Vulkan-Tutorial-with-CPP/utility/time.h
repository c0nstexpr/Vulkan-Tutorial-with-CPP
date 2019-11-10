#pragma once

#include <chrono>

namespace utility::time
{
	using namespace std::chrono;

	inline constexpr auto steady_clock_timer = steady_clock::now;

	inline constexpr auto high_resolution_clock_timer = high_resolution_clock::now;

	inline constexpr auto system_clock_timer = system_clock::now;
}
