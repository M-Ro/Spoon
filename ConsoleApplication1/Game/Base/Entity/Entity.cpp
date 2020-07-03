#include "Entity.h"

#include "../GameName.h"

void Entity::Draw()
{
	game->renderer->DrawModel(modelname, position, rotation);
}