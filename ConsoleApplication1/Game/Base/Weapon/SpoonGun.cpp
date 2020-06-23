#include "SpoonGun.h"
#include "../Entity/Projectiles/Spoon.h"
#include "../../../auxiliary/Time.h"

SpoonGun::SpoonGun(Entity* owner) : Weapon(owner)
{
	this->owner = owner;

	recoilLength = 6;
	recoilSpeed = 50;
	lastShot = Time::GetCurrentTimeMillis() + recoilLength * recoilSpeed;
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
	float spread_x = 0.10;
	float spread_y = 0.10;
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

glm::vec3 SpoonGun::GetRecoil()
{
	float state = (Time::GetCurrentTimeMillis() - lastShot) / recoilSpeed;
	if (state > recoilLength-1 || lastShotType == 2)
		return glm::vec3(0,0,0);
	/*
	int idx_lower = floor(state);
	int idx_higher = ceil(state);
	float weight_higher = state - (float)idx_lower;
	float weight_lower = (float)idx_higher - state;
	glm::vec3 v = owner->rotation * (weight_lower * posOffsets[idx_lower] + weight_higher * posOffsets[idx_higher]);
	*/
	glm::vec3 v = owner->rotation * (state * -7.5873f + state * state * 2.59524f - state * state * state * 0.22222f);
	return v;
}

glm::vec3 SpoonGun::GetRecoilAngle(glm::vec2 *rotation)
{
	float state = (Time::GetCurrentTimeMillis() - lastShot)/500.0f;
	if (state > 2.0 || lastShotType == 1)
		return glm::vec3(rotation->x, rotation->y, 0);

	float impulse = 0.5*state*pow(2.71, (1.0f-5.0f*state));
	return glm::vec3(rotation->x, rotation->y + impulse, 0);
}

SpoonGun::~SpoonGun()
{

}