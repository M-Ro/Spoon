#include <map>
#include <string>

#include "EntityMapper.h"

#include "Entity/Spawner.h"
#include "Entity/Skull.h"

template<typename T> Entity* createInstance() { return new T; }

std::map<std::string, Entity* (*)()> ent_mappings = {
	{ "skull", (createInstance<Skull>) },
	{ "spawner", (createInstance<Spawner>) },
};

Entity *EntityMapper::EntityFromClassname(std::string const& classname)
{
	if (ent_mappings.count(classname)) {
		return ent_mappings[classname]();
	}
	else {
		return nullptr;
	}
}