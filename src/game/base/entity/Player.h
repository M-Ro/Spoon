#pragma once

#include "Entity.h"
#include "../../../input/InputHandler.h"
#include <glm/glm.hpp>

const float MOVEMENT_SPEED = 200.0;

class Player : public Entity
{
	friend class GameName;
public:

	Player(InputHandler *input);
	virtual ~Player();

	void Update(float deltaTime);

	void Hurt(float dmg);

	void Die();

private:

	void HandlePlayerInput(float deltaTime);

	void ProjectView();

	glm::vec2 cam_rot;

	InputHandler *input;

	// For movement
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 dir;

};