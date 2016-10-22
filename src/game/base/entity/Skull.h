#pragma once

#include "Monster.h"

class Skull : public Monster
{
public:

	Skull();
	virtual ~Skull();
	void Update(float deltaTime);
	void Hurt(float dmg);

	void Die();

protected:
	float turnrate;
	float speed;
};