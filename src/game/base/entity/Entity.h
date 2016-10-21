#pragma once

#include <string>

#include <glm/glm.hpp>

static int last_id = 0;

class Entity
{
	friend class GameName;
public:

	enum Team { Player, Monster };

	Entity()
	{
		id = ::last_id;
		::last_id++;

		position = glm::vec3(0, 0, 0);
		rotation = glm::vec3(0, 0, 0);
		velocity = glm::vec3(0, 0, 0);
	}

	virtual void Update(float deltaTime) = 0;

	virtual void Hurt(float dmg) = 0;

	virtual void Die() = 0;

protected:

	unsigned long id;

	float health;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 velocity;

	std::string modelname;

	bool destroy;

	Team team;

};