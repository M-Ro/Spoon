#pragma once
#include "Weapon.h"

class SpoonGun : public Weapon
{
public:
	SpoonGun(Entity* owner);
	~SpoonGun();
	int primary_firerate = 150;
	int secondary_firerate = 500;

	int recoilSpeed;
	int recoilLength;

	void PrimaryFire();
	void SecondaryFire();
	void GetRecoilPos(glm::vec3* result);
	void GetRecoilAngle(glm::vec2 *rotation, glm::vec3* result);
};