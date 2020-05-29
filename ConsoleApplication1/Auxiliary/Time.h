#pragma once

class Time
{
public:
	static void Initialise();

	static void Update();

	static long long GetStartTimeMillis();

	static long long GetCurrentTimeMillis();

	static long long GetTimeSinceStartMillis();
};