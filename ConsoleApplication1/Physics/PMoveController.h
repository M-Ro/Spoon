#pragma once

#include "MovementController.h"

class PMoveController : public MovementController
{
public:
	PMoveController(Entity* tracked);
	~PMoveController();

	virtual void Update(float deltaTime, glm::vec3 cDir);

	void SetCapsuleSize(physx::PxF32 radius, physx::PxF32 height);

	bool OnFloor();
protected:

};