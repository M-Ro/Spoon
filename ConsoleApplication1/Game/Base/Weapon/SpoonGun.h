#pragma once
#include "Weapon.h"

class SpoonGun : public Weapon
{
public:
	SpoonGun(Entity* owner);
	~SpoonGun();
	float primary_firerate = 150.0f;
	float secondary_firerate = 500.0f;

	float recoilSpeed;
	int recoilLength;

	void PrimaryFire();
	void SecondaryFire();
	void GetRecoilPos(glm::vec3* result);
	void GetRecoilAngle(glm::vec2 *rotation, glm::vec3* result);
protected:
	long long spawntime;
};