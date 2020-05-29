#pragma once

#include "Time.h"

class FpsCounter
{
public:
	static void Tick();
	
	static long long fps;
private:
	static long long frames;
	static long long last_second;

};