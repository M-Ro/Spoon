#include "Entity.h"

#include "../../Globals.h"

void Entity::Draw()
{
	renderer->DrawModel(modelname, position, rotation);
}