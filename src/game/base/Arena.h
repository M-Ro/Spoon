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

	bool Intersects(Entity *a, Entity *b);
	bool IntersectsWorld(Entity *a);
	bool PointIntersectsWorld(const glm::vec3 &a);

	bool OnFloor(Entity *a);

	inline void SetDrawBBoxes() { drawBBoxes = !drawBBoxes; }

private:

	void RunPhysics(float deltaTime);

	std::map<long, Entity *> entities;
	float spawn_timer;
	std::string modelname;

	bool drawBBoxes;
};