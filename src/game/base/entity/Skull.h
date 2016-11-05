#pragma once

#include "Monster.h"

class Skull : public Monster
{
public:

	Skull();
	virtual ~Skull();
	void Touch(Entity *other);
	void Update(float deltaTime);
	void Hurt(Entity *attacker, float dmg);
	void SendEntity();

	void Die();

protected:
	float turnrate;
	float speed;
};