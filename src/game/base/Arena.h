#pragma once

#include <string>
#include <map>

#include "entity/Entity.h"

class Arena
{
public:

	Arena(std::string &name);

	~Arena();

	void Update();

	void Draw();

private:

	std::map<long, Entity *> entities;

	std::string modelname;
};