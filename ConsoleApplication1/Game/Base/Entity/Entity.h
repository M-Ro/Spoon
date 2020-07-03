#pragma once

#include <string>

#include <glm/glm.hpp>

#include "../CollisionModel.h"

static int last_id = 0;

class Arena;

class Entity
{
	friend class GameName;
public:

	Arena *arena;
	Entity *owner;
	Entity *enemy;
	
	std::string classname;

	unsigned long id;

	float health;
	long long attack_finished;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 velocity;

	CollisionModel cmodel;
	bool solid;
	bool onFloor;

	std::string modelname;
	float net_nextSendEntity;
	float net_lastUpdate;

	enum Team { Player, Monster };
	enum MovementType { Static, Walk, Fly, Manual }; // Used for physics
	float maxSpeed; // Max movement speed

	Team team;
	MovementType moveType;

	bool destroy; // marked for deletion

	Entity()
	{
		id = ::last_id;
		::last_id++;

		classname = "undefined";
		destroy = false;
		position = glm::vec3(0, 0, 0);
		rotation = glm::vec3(0, 0, 0);
		velocity = glm::vec3(0, 0, 0);
		net_nextSendEntity = 0.0f;
	}

	virtual void Update(float deltaTime) = 0;

	virtual void Draw();

	virtual void Touch(Entity *other) = 0;

	virtual void Hurt(Entity *attacker, float dmg) = 0;

	virtual void Die() = 0;

	const long GetId() const { return id; }

	const std::string &GetClassname() const { return classname; }
};

#include "../Arena.h"