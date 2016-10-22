#pragma once

#include "Entity.h"

class Monster : public Entity
{
public:

	Monster() : Entity() {  team = Team::Monster; }
	virtual ~Monster() {};

protected:
};