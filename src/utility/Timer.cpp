#include "Precomp.h"
#include "Timer.h"
#include <chrono>

typedef std::chrono::duration<uint32_t, std::milli> milliseconds;
typedef std::chrono::duration<uint32_t, std::micro> microseconds;
typedef std::chrono::duration<uint32_t, std::nano> nanoseconds;

Timer::Timer()
{
	update_real_time();
	set_time(0);
	resolution = TimerResolution::MILLISECOND;
}

Timer::Timer(TimerResolution resolution):Timer()
{
	resolution = resolution;
}

Timer::Timer(uint32_t begin_time)
{
	set_time(begin_time);
}

Timer::Timer(uint32_t begin_time, TimerResolution resolution):Timer(begin_time)
{
	resolution = resolution;
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
	update_real_time();
	this->start_time = this->real_time + time;
	this->time = time;
	this->last_time = time;
}

void Timer::update_real_time()
{
	const std::chrono::high_resolution_clock::time_point & rt = std::chrono::high_resolution_clock::now();

	switch (resolution)
	{
	case TimerResolution::NANOSECOND:
	{
		nanoseconds time = std::chrono::duration_cast<nanoseconds>(rt.time_since_epoch());
		real_time = time.count();
	}
	break;
	
	case TimerResolution::MICROSECOND:
	{
		microseconds time = std::chrono::duration_cast<microseconds>(rt.time_since_epoch());
		real_time = time.count();
	}
	break;
	
	default:
	case TimerResolution::MILLISECOND:
	{
		milliseconds time = std::chrono::duration_cast<milliseconds>(rt.time_since_epoch());
		real_time = time.count();
	}
	break;
	}
}

void Timer::tick()
{
	update_real_time();
	last_time = time;
	time = real_time - start_time;
}