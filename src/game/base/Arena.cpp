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
	RunPhysics(deltaTime);

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

/* Returns true if any point of a intersects b */
bool Arena::Intersects(Entity *a, Entity *b)
{
	glm::vec3 a_mins = glm::vec3(a->position.x-a->bbox.x/2, a->position.y-a->bbox.y/2, a->position.z-a->bbox.z/2);
	glm::vec3 a_maxs = glm::vec3(a->position.x+a->bbox.x/2, a->position.y+a->bbox.y/2, a->position.z+a->bbox.z/2);
	glm::vec3 b_mins = glm::vec3(b->position.x-b->bbox.x/2, b->position.y-b->bbox.y/2, b->position.z-b->bbox.z/2);
	glm::vec3 b_maxs = glm::vec3(b->position.x+b->bbox.x/2, b->position.y+b->bbox.y/2, b->position.z+b->bbox.z/2);

	return (a_mins.x <= b_maxs.x && a_maxs.x >= b_mins.x) &&
		 (a_mins.y <= b_maxs.y && a_maxs.y >= b_mins.y) &&
		 (a_mins.z <= b_maxs.z && a_maxs.z >= b_mins.z);
}

bool Arena::IntersectsWorld(Entity *a)
{
	//glm::vec3 w_mins = glm::vec3(-224, -32, -224);
	//glm::vec3 w_maxs = glm::vec3(224, 0, 224);

	return false;
}

bool Arena::PointIntersectsWorld(const glm::vec3 &a)
{
	glm::vec3 w_mins = glm::vec3(-224, -32, -224);
	glm::vec3 w_maxs = glm::vec3(224, 0, 224);

	return  (a.x >= w_mins.x && a.x <= w_maxs.x) &&
			(a.y >= w_mins.y && a.y <= w_maxs.y) &&
			(a.z >= w_mins.z && a.z <= w_maxs.z);
}

void Arena::RunPhysics(float deltaTime)
{
	const float gravity = 20;
	const float airFriction = 1.5;
	const float friction = 3;
	const float termVel = 1.5;
	
	/* Apply world forces (gravity, friction... ) */
	for (auto& kv : entities)
	{
		/* We don't apply gravity or other forces to flying entities, only walkers */
		Entity *ent = kv.second;
		if(!ent)
			return;

		if(ent->moveType == Entity::MovementType::Walk)
		{
			glm::vec3 xzVel = glm::vec3(ent->velocity.x, 0, ent->velocity.z);
			glm::vec3 xzVelN = glm::vec3(0, 0, 0);
			if(glm::length(xzVel) != 0)
				xzVelN = glm::normalize(xzVel);

			ent->onFloor = OnFloor(ent);

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
			ent->position += ent->velocity;
		}
	}

	/* This checks for entity:entity collisions
	 * To avoid processing each collision twice (once for each entity)
	 * we copy the list and remove the entity after comparing it with all other entities */

	std::map<long, Entity *> tmpEnts(entities);
	for (auto it = tmpEnts.cbegin(); it != tmpEnts.cend();)
	{
		Entity *a = it->second;
		if(!a->solid)
			continue;

		for (auto& kv2 : tmpEnts)
		{
			Entity *b = kv2.second;
			if(!b->solid)
				continue;

			if(Intersects(a, b))
			{
				a->Touch(b);
				b->Touch(a);
			}
		}

		tmpEnts.erase(it++);
	}
}

bool Arena::OnFloor(Entity *ent)
{
	glm::vec3 e_pos = ent->position;
	glm::vec3 e_bbox = ent->bbox;

	glm::vec3 p[4];
	p[0] = glm::vec3(e_pos.x+e_bbox.x/2, e_pos.y-e_bbox.y/2, e_pos.z-e_bbox.z/2);
	p[1] = glm::vec3(e_pos.x+e_bbox.x/2, e_pos.y-e_bbox.y/2, e_pos.z+e_bbox.z/2);
	p[2] = glm::vec3(e_pos.x-e_bbox.x/2, e_pos.y-e_bbox.y/2, e_pos.z-e_bbox.z/2);
	p[3] = glm::vec3(e_pos.x-e_bbox.x/2, e_pos.y-e_bbox.y/2, e_pos.z+e_bbox.z/2);

	for(int i=0; i<4; i++)
		if(PointIntersectsWorld(p[i]))
			return true;


	return false;
}