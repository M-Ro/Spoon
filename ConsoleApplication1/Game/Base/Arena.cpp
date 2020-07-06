#include <iostream>

#include "Arena.h"
#include "entity/skull.h"
#include "../Globals.h"
#include "entity/Spawner.h"

Arena::Arena(const std::string &name)
{
	modelname = name;
	spawn_timer = 0.0f;
	drawBBoxes = false;

	arena_cmodel = CollisionModel(CollisionModel::ModelType::Box, glm::vec3(1200, 8, 1200));
	Spawner* s = new Spawner();
	AddEntity(s);

	audio->PlayAt(glm::vec3(0, 0, 0), "ambience/loop0", 1.0, true);
}

Arena::~Arena()
{
	// Delete all entities
	for (auto& kv : entities) {
		if (kv.second) {
			delete kv.second;
		}
	}
}

void Arena::Update(float deltaTime)
{
	RunPhysics(deltaTime);

	// Purge dead entities
	for (auto it = entities.cbegin(); it != entities.cend();)
	{
		if (it->second->destroy) {
			delete it->second;
			entities.erase(it++);
		}
		else
			it++;
	}

	// Update all entities
	for (auto& kv : entities)
		kv.second->Update(deltaTime);
}

void Arena::Draw()
{
	// Draw arena
	glm::vec3 pos(0.0, 0.0, 0.0);
	//game->renderer->DrawModel("arena", pos, pos);

	// Draw all entities
	for (auto& kv : entities)
	{
		kv.second->Draw();
		if(drawBBoxes)
			renderer->DrawCModel((int)kv.second->cmodel.type, kv.second->position, kv.second->cmodel.size, kv.second->cmodel.radius);
	}
}

Entity *Arena::FindEntityById(long id)
{
	if(entities.count(id))
		return entities.at(id);
	
	return 0;
}

std::list<Entity *> *Arena::FindEntityByClass(std::string classname)
{
	std::list<Entity *> *list = 0;

	for (auto& kv : entities)
	{
		Entity *e = kv.second;
		if(classname == e->GetClassname())
		{
			if(!list)
				list = new std::list<Entity *>();

			list->push_back(e);
		}
	}

	return list;
}

// Returns 1 on success
bool Arena::AddEntity(Entity *entity)
{
	if(!entity)
	{
		std::cout << "Arena::AddEntity attempted to add null entity" << std::endl;
		return 0;
	}

	long id = entity->GetId();

	if(entities.count(id) > 0)
	{
		std::cout << "Arena::AddEntity attempted to add entity with duplicate id: " << id << std::endl;
		return 0;
	}

	entity->arena = this;
	entities[id] = entity;

	return 1;
}

// Returns 1 on success
bool Arena::RemoveEntity(const long id)
{
	if(!entities.count(id))
	{
		std::cout << "Arena::RemoveEntity no entity at id: " << id << std::endl;
		return 0;
	}

	entities[id]->arena = 0;
	entities.erase(id);

	return 1;
}

void Arena::RunPhysics(float deltaTime)
{
	const float gravity = 80;
	const float airFriction = 20;
	const float friction = 500;
	const float termVel = 110;
	
	/* Apply world forces (gravity, friction... ) */
	for (auto& kv : entities)
	{
		/* We don't apply gravity or other forces to flying entities, only walkers */
		Entity *ent = kv.second;
		if(!ent)
			continue;
		if(ent->classname == "player")
			continue;

		if(ent->moveType == Entity::MovementType::Walk)
		{
			glm::vec3 xzVel = glm::vec3(ent->velocity.x, 0, ent->velocity.z);
			glm::vec3 xzVelN = glm::vec3(0, 0, 0);
			if(glm::length(xzVel) != 0)
				xzVelN = glm::normalize(xzVel);

			ent->onFloor = ent->cmodel.OnFloor(ent->position, arena_cmodel);

			/* If airbound, apply airFriction and gravity */
			if(!ent->onFloor)
			{
				ent->velocity -= (deltaTime * xzVelN * airFriction);
				ent->velocity.y -= deltaTime * gravity;

				if(ent->velocity.y < -termVel)
					ent->velocity.y = -termVel;
			}
			else // On Floor
			{
				if(ent->velocity.y < 0) // Stop ent going through floor
					ent->velocity.y = 0;

				ent->velocity -= (deltaTime * xzVelN * friction);
			}

			/* Apply velocity to position */
			ent->position += ent->velocity * deltaTime;
		}
		else if(ent->moveType == Entity::MovementType::Fly)
		{
			ent->position += ent->velocity * deltaTime;
		}
	}

	/* This checks for entity:entity collisions
	 * To avoid processing each collision twice (once for each entity)
	 * we copy the list and remove the entity after comparing it with all other entities */
	std::map<long, Entity *> tmpEnts(entities);
	for (auto it = tmpEnts.cbegin(); it != tmpEnts.cend();)
	{
		Entity *a = it->second;
		tmpEnts.erase(it++);

		if(!a->solid)
			continue;

		for (auto& kv2 : tmpEnts)
		{
			Entity *b = kv2.second;
			if(!b->solid)
				continue;

			if(a->cmodel.Intersects(a->position, b->position, b->cmodel))
			{
				a->Touch(b);
				b->Touch(a);
			}
		}
	}
}
