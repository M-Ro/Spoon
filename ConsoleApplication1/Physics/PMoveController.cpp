#include <iostream>

#include "PMoveController.h"
#include "Physics.h"

// https://docs.nvidia.com/gameworks/content/gameworkslibrary/physx/guide/Manual/CharacterControllers.html

PMoveController::PMoveController(Entity *tracked) : MovementController(tracked)
{
	physx::PxCapsuleControllerDesc desc;
	desc.height = 32.0f;
	desc.radius = 16.0f;
	desc.stepOffset = 2.0f;
	desc.position = physx::PxExtendedVec3(tracked->position.x, tracked->position.y, tracked->position.z);
	desc.material = Physics::GetDefaultMat();
	desc.contactOffset = 0.0001f;

	this->controller = Physics::GetControllerManager()->createController(desc);

	if (!this->controller) {
		std::cout << "Error: Failed to create PMove controller" << std::endl;
	}
}

PMoveController::~PMoveController()
{

}

void PMoveController::SetCapsuleSize(physx::PxF32 radius, physx::PxF32 height)
{
	if (!this->controller) {
		return;
	}

	this->controller->resize(height);
}

void PMoveController::Update(float deltaTime, glm::vec3 cDir)
{
	physx::PxVec3 disp(cDir.x, cDir.y, cDir.z);

	physx::PxControllerCollisionFlags collisionFlags =
		this->controller->move(disp, 0.01f, deltaTime,
			nullptr);
}

bool PMoveController::OnFloor()
{
	physx::PxExtendedVec3 extended = this->controller->getPosition();
	physx::PxVec3 ray_origin(extended.x, extended.y-32.0f, extended.z);

	physx::PxRaycastBuffer hit; // output

	bool collides = Physics::GetScene()->raycast(ray_origin, physx::PxVec3(0, -1, 0), 0.1f, hit, physx::PxHitFlag::eMESH_ANY);
	
	return collides;
}