#include "Time.h"

#include <ctime>

struct timespec ts_start;
struct timespec ts_current;

long time_start_ms;
long time_current_ms;

void Time::Initialise()
{
	/* Get start time */
	clock_gettime(CLOCK_MONOTONIC, &ts_start);

	time_start_ms = ts_start.tv_sec * 1000;
    time_start_ms += ts_start.tv_nsec / 1000000;
}

void Time::Update()
{
	/* Get current time */
	clock_gettime(CLOCK_MONOTONIC, &ts_current);
	time_current_ms = ts_current.tv_sec * 1000;
    time_current_ms += ts_current.tv_nsec / 1000000;
}

long Time::GetStartTimeMillis()
{
	return time_start_ms;
}

long Time::GetCurrentTimeMillis()
{
	return time_current_ms;
}

long Time::GetTimeSinceStartMillis()
{
	return time_current_ms - time_start_ms;
}
