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
	Timer(uint32_t begin_time = 0, TimerResolution resolution = TimerResolution::MILLISECOND);

	virtual ~Timer();

	uint32_t get_time();
	uint32_t get_start_time();
	uint32_t get_delta_time();
	float get_delta_time_ms();
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
