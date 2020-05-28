#pragma once

#include "Monster.h"

class Spawner : public Monster
{
public:

	Spawner();
	virtual ~Spawner();
	void Touch(Entity *other);
	void Update(float deltaTime);
	void Hurt(Entity *attacker, float dmg);
	void SendEntity();
	void SpawnSkulls(int amount);

	void Die();

protected:
	float speed;
	float spawn_timer;
};