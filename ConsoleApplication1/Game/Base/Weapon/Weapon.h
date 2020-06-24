#pragma once
#include "../Entity/Entity.h"

class Weapon
{
public:
	Weapon(Entity* owner);
	~Weapon() {}
	long long lastShot = 0;
	int lastShotType = 0;
	Entity* owner;

	virtual void PrimaryFire() = 0;
	virtual void SecondaryFire() = 0;
	virtual void GetRecoilPos(glm::vec3* result) = 0;
	virtual void GetRecoilAngle(glm::vec2 *rotation, glm::vec3 *result) = 0;
};