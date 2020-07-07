#pragma once

#include "../Game/Base/Entity/Entity.h"

#include "PxPhysicsAPI.h"

class MovementController
{
public:
	MovementController(Entity* tracked);
	virtual ~MovementController();

	void SetTracked(Entity* newEnt);

	virtual void Update(float deltaTime, glm::vec3 cDir) = 0;

	virtual void Teleport(glm::vec3 const &pos);

	glm::vec3 GetPosition();

protected:

	Entity* tracked;
	physx::PxController* controller;
};