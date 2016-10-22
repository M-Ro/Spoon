#pragma once

#include <string>
#include <list>
#include <map>

#include "entity/Entity.h"

class Arena
{
public:

	Arena(const std::string &name);

	~Arena();

	void Update(float deltaTime);

	void Draw();

	Entity *FindEntityById(long id);

	std::list<Entity *> *FindEntityByClass(std::string classname);

	bool AddEntity(Entity *entity);

	bool RemoveEntity(const long id);

private:

	std::map<long, Entity *> entities;

	std::string modelname;
};