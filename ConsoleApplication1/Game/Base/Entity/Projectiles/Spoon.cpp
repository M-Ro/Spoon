#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "Spoon.h"
#include "../../../../auxiliary/Time.h"
#include "../../../definitions.h"

Spoon::Spoon(Entity *owner) : Projectile(owner)
{
	classname = "projectile_spoon";
	modelname = "spoon";
	lifetime = 4000;
	speed = 500;
	cmodel = CollisionModel(CollisionModel::ModelType::Box, glm::vec3(6, 6, 6));
	solid = true;
}

void Spoon::Update(float deltaTime)
{
	if(Time::GetCurrentTimeMillis() > spawntime + lifetime)
		Die();

	velocity = dir * speed;
	position += velocity * deltaTime;

	return;
}

Spoon::~Spoon()
{

}