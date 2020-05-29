#include "fpscounter.h"

long long FpsCounter::fps;
long long FpsCounter::frames;
long long FpsCounter::last_second;

void FpsCounter::Tick()
{
	frames++;

	if(Time::GetCurrentTimeMillis() >= last_second + 1000)
	{
		fps = frames;
		frames = 0;
		last_second = Time::GetCurrentTimeMillis();
	}
}