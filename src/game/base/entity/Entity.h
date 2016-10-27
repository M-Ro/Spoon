#pragma once

#include <string>

#include <glm/glm.hpp>

#include "../GameName.h"

static int last_id = 0;

class Entity
{
	friend class GameName;
public:

	std::string classname;

	unsigned long id;

	float health;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 velocity;

	glm::vec3 bbox;

	std::string modelname;

	enum Team { Player, Monster };
	enum MovementType { Static, Walk, Fly }; // Used for physics

	Team team;
	MovementType moveType;
	bool solid;

	bool onFloor;

	Entity()
	{
		id = ::last_id;
		::last_id++;

		classname = "undefined";
		position = glm::vec3(0, 0, 0);
		rotation = glm::vec3(0, 0, 0);
		velocity = glm::vec3(0, 0, 0);
	}

	virtual void Update(float deltaTime) = 0;

	virtual void Draw()
	{
		game->renderer->DrawModel(modelname, position, rotation);
	}

	virtual void Touch(Entity *other) = 0;

	virtual void Hurt(float dmg) = 0;

	virtual void Die() = 0;

	const long GetId() const { return id; }

	const std::string &GetClassname() const { return classname; }

protected:

	bool destroy;
};