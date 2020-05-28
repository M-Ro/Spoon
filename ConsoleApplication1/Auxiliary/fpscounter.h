#pragma once

#include "Time.h"

class FpsCounter
{
public:
	static void Tick();
	
	static long fps;
private:
	static long frames;
	static long last_second;

};