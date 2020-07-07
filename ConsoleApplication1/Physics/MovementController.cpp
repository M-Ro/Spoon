#include <iostream>

#include "MovementController.h"

MovementController::MovementController(Entity* tracked)
{
	this->controller = nullptr;
	this->tracked = tracked;
}

MovementController::~MovementController()
{
	if (this->controller) {
		this->controller->release();
	}
}

void MovementController::SetTracked(Entity* newEnt)
{
	this->tracked = newEnt;
}

void MovementController::Teleport(glm::vec3 const& pos)
{
	if (!this->controller) {
		return;
	}

	this->controller->setPosition(
		physx::PxExtendedVec3(tracked->position.x, tracked->position.y, tracked->position.z)
	);
}

glm::vec3 MovementController::GetPosition()
{
	if (!this->controller) {
		std::cout << "Warning: GetPosition() to MovementController with null PxController!" << std::endl;
		return glm::vec3(0, 0, 0);
	}

	physx::PxExtendedVec3 pos = this->controller->getPosition();
	return glm::vec3(pos.x, pos.y, pos.z);
}