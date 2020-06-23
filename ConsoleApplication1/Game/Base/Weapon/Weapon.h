#pragma once
#include "../Entity/Entity.h"

class Weapon
{
public:
	Weapon(Entity* owner);
	~Weapon() {}
	long long lastShot = 0.0f;
	int lastShotType = 0;
	Entity* owner;

	virtual void PrimaryFire() = 0;
	virtual void SecondaryFire() = 0;
	virtual glm::vec3 GetRecoil() = 0;
	virtual glm::vec3 GetRecoilAngle(glm::vec2 *rotation) = 0;
protected:
	long long spawntime;
};