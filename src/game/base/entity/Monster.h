#pragma once

#include "Entity.h"

class Monster : public Entity
{
public:

	Monster() {  team = Team::Monster; }
	virtual ~Monster() = 0;

protected:
};