#pragma once

#include "Entity/Entity.h"

class EntityMapper
{
public:
	static Entity* EntityFromClassname(std::string const& classname);
};