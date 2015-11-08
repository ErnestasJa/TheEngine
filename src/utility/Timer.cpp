#include "Precomp.h"
#include "Timer.h"
#include <chrono>

typedef std::chrono::duration<uint32_t, std::milli> milliseconds;
typedef std::chrono::duration<uint32_t, std::micro> microseconds;
typedef std::chrono::duration<uint32_t, std::nano> nanoseconds;

Timer::Timer(uint32_t begin_time, TimerResolution resolution)
{
	resolution = TimerResolution::MILLISECOND;
	set_time(begin_time);
}

Timer::~Timer()
{
}

uint32_t Timer::get_time()
{
	return time;
}

uint32_t Timer::get_real_time()
{
	return real_time;
}

uint32_t Timer::get_start_time()
{
	return start_time;
}

uint32_t Timer::get_delta_time()
{
	return time - last_time;
}

void Timer::set_time(uint32_t time)
{
	this->real_time = get_system_real_time();
	this->start_time = this->real_time + time;
	this->time = time;
	this->last_time = time;
}

uint32_t Timer::get_system_real_time()
{
	const std::chrono::high_resolution_clock::time_point & rt = std::chrono::high_resolution_clock::now();

	switch (resolution)
	{
	case TimerResolution::NANOSECOND:
		return std::chrono::duration_cast<nanoseconds>(rt.time_since_epoch()).count();
	case TimerResolution::MICROSECOND:
		return std::chrono::duration_cast<microseconds>(rt.time_since_epoch()).count();
	default:
		return std::chrono::duration_cast<milliseconds>(rt.time_since_epoch()).count();
	}
}

void Timer::tick()
{
	real_time = get_system_real_time();
	last_time = time;
	time = real_time - start_time;
}