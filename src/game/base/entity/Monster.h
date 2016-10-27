#pragma once

#include "Entity.h"

class Monster : public Entity
{
public:

	Monster() : Entity() {  team = Team::Monster; }
	virtual ~Monster() {};

	virtual void Touch(Entity *other) = 0;

protected:
};