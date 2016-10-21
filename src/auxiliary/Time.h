#pragma once

class Time
{
public:
	static void Initialise();

	static void Update();

	static long GetStartTimeMillis();

	static long GetCurrentTimeMillis();

	static long GetTimeSinceStartMillis();
};