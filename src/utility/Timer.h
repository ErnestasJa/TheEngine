#pragma once

enum class TimerResolution
{
	NANOSECOND,
	MICROSECOND,
	MILLISECOND
};

class Timer
{
protected:
	uint32_t start_time;
	uint32_t time;
	uint32_t real_time;
	uint32_t last_time;

	void update_real_time();

	TimerResolution resolution;

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
};

typedef std::shared_ptr<Timer> timer_ptr;
typedef std::shared_ptr<Timer> TimerPtr;
