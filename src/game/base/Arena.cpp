#include <iostream>

#include "Arena.h"
#include "entity/skull.h"
#include "../Game.h"

Arena::Arena(const std::string &name)
{
	modelname = name;
	spawn_timer = 0.0f;
}

Arena::~Arena()
{

}

void Arena::Update(float deltaTime)
{
	// Update all entities
	for (auto& kv : entities)
		kv.second->Update(deltaTime);

	spawn_timer -= deltaTime;
	if(spawn_timer < 0.0){
		Skull * s = new Skull();
		AddEntity(s);
		spawn_timer = 10.0f;
	}
}

void Arena::Draw()
{
	// Draw arena
	glm::vec3 pos(0.0, 0.0, 0.0);
	game->renderer->DrawModel("arena", pos, pos);

	// Draw all entities
	for (auto& kv : entities)
		kv.second->Draw();
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

	entities.erase(id);

	return 1;
}