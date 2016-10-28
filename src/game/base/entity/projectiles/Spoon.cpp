#include <iostream>
#include "Spoon.h"

Spoon::Spoon(Entity *owner) : Projectile(owner)
{
	classname = "projectile_spoon";
	modelname = "spoon";
	bbox = glm::vec3(6, 6, 6);
	lifetime = 4000;
	speed = 500;
	solid = true;
}

Spoon::~Spoon()
{

}