#include "SpoonGun.h"
#include "../Entity/Projectiles/Spoon.h"
#include "../../../auxiliary/Time.h"

SpoonGun::SpoonGun(Entity* owner) : Weapon(owner)
{
	this->owner = owner;

	recoilLength = 6;
	recoilSpeed = 50;
	lastShot = Time::GetCurrentTimeMillis() + (long long)recoilLength * recoilSpeed;

}

void SpoonGun::PrimaryFire() 
{
	if (owner->attack_finished > Time::GetCurrentTimeMillis())
		return;
	lastShotType = 1;
	lastShot = Time::GetCurrentTimeMillis();

	glm::vec3 front = owner->rotation;
	glm::vec3 right = glm::vec3(-front.z, 0, front.x);
	glm::normalize(right);
	glm::vec3 up = -1.0f * glm::cross(front, right);

	Spoon* spoon = new Spoon(owner);
	spoon->dir = owner->rotation;
	spoon->position = owner->position - 4.0f*up;

	spoon->rotation = glm::vec3(owner->rotation.x, owner->rotation.y, 0);
	owner->arena->AddEntity(spoon);

	owner->attack_finished = Time::GetCurrentTimeMillis() + primary_firerate;
}

void SpoonGun::SecondaryFire()
{
	if (owner->attack_finished > Time::GetCurrentTimeMillis())
		return;
	lastShotType = 2;
	lastShot = Time::GetCurrentTimeMillis();
	glm::vec3 front = owner->rotation;
	glm::vec3 right = glm::vec3(-front.z, 0, front.x);
	glm::normalize(right);
	glm::vec3 up = -1.0f * glm::cross(front, right);
	int pellets = 10;
	float spread_x = 0.10f;
	float spread_y = 0.10f;
	while (pellets-- > 0) {
		float spread_hor = spread_x * rand() / RAND_MAX - spread_x * 0.5f;
		float spread_ver = spread_y * rand() / RAND_MAX - spread_y * 0.5f;
		glm::vec3 dir = front + spread_hor*right + spread_ver*up;
		Spoon* spoon = new Spoon(owner);
		spoon->dir = dir;
		spoon->position = owner->position - 4.0f * up;;
		spoon->rotation = glm::vec3(owner->rotation.x, owner->rotation.y, 0);
		owner->arena->AddEntity(spoon);
	}
	owner->attack_finished = Time::GetCurrentTimeMillis() + secondary_firerate;
}

void SpoonGun::GetRecoilPos(glm::vec3* result)
{
	float state = (float)(Time::GetCurrentTimeMillis() - lastShot) / recoilSpeed;
	if (state > recoilLength - 1 || lastShotType == 2)
	{
		result->x = owner->position.x;
		result->y = owner->position.y;
		result->z = owner->position.z;
		return;
	}
	float impulse = (state * -7.5873f + state * state * 2.59524f - state * state * state * 0.22222f);	//	 A simple function that happens to give a roughly nice behaviour for recoil when we go from 0 to 6 (recoilLength)
	result->x = owner->position.x + owner->rotation.x * impulse;
	result->y = owner->position.y + owner->rotation.y * impulse;
	result->z = owner->position.z + owner->rotation.z * impulse;
	//glm::vec3 v = owner->position + owner->rotation * (state * -7.5873f + state * state * 2.59524f - state * state * state * 0.22222f);
	//return v;
}

void SpoonGun::GetRecoilAngle(glm::vec2 *rotation, glm::vec3* result)
{
	float state = (Time::GetCurrentTimeMillis() - lastShot)/500.0f;
	if (state > 2.0 || lastShotType == 1) {
		result->x = rotation->x;
		result->y = rotation->y;
		return;
	}
		

	float impulse = 0.5f*state*pow(2.71f, (1.0f-5.0f*state));
	result->x = rotation->x;
	result->y = rotation->y+ impulse;
	return;
}

SpoonGun::~SpoonGun()
{

}