#pragma once

enum class TimerResolution
{
	NANOSECOND,
	MICROSECOND,
	MILLISECOND
};

class Timer
{
public:

	Timer();
	Timer(uint32_t begin_time);
	Timer(uint32_t begin_time, TimerResolution resolution);
	Timer(TimerResolution resolution);

	virtual ~Timer();

	uint32_t get_time();
	uint32_t get_start_time();
	uint32_t get_delta_time();
	void set_time(uint32_t time);

	uint32_t get_real_time();

	void tick();

protected:
	uint32_t get_system_real_time();

protected:
	uint32_t start_time;
	uint32_t time;
	uint32_t real_time;
	uint32_t last_time;
	TimerResolution resolution;
};

typedef std::shared_ptr<Timer> timer_ptr;
typedef std::shared_ptr<Timer> TimerPtr;
