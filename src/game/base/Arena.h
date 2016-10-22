#pragma once

#include <string>
#include <list>
#include <map>

#include "entity/Entity.h"

class Arena
{
public:

	Arena(std::string &name);

	~Arena();

	void Update();

	void Draw();

	Entity *FindEntityById(long id);

	std::list<Entity *> *FindEntityByClass(std::string classname);

private:

	std::map<long, Entity *> entities;

	std::string modelname;
};