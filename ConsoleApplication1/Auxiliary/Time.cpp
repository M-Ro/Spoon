#include "Time.h"

#include <chrono>

using namespace std::chrono;

std::chrono::time_point<std::chrono::steady_clock> start;
std::chrono::time_point<std::chrono::steady_clock> current;

long long time_start_ms;
long long time_current_ms;

void Time::Initialise()
{
	/* Get start time */
	start = steady_clock::now();
	auto start_ms = time_point_cast<milliseconds>(start);
	auto value = start_ms.time_since_epoch();
	time_start_ms = value.count();
}

void Time::Update()
{
	/* Get current time */
	current = steady_clock::now();

	auto current_ms = time_point_cast<milliseconds>(current);
	auto value = current_ms.time_since_epoch();
	time_current_ms = value.count();
}

long long Time::GetStartTimeMillis()
{
	return time_start_ms;
}

long long Time::GetCurrentTimeMillis()
{
	return time_current_ms;
}

long long Time::GetTimeSinceStartMillis()
{
	return time_current_ms - time_start_ms;
}
