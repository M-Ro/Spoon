#pragma once

#include <iostream>

// EFX Effects

class Effect
{
public:
	Effect(const std::string &name);
	~Effect();

	bool ParseEffect(const std::string &name);

private:
	bool valid;
};