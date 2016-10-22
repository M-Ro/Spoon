#include "fpscounter.h"

long FpsCounter::fps;
long FpsCounter::frames;
long FpsCounter::last_second;

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