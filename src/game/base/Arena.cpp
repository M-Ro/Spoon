#include "Arena.h"

Arena::Arena(std::string &name)
{

}

Arena::~Arena()
{

}

void Arena::Update()
{

}

void Arena::Draw()
{
	
}

Entity *Arena::FindEntityById(long id)
{
	for (auto& kv : entities)
	{
		Entity *e = kv.second;
		if(id == e->GetId())
			return e;
	}
	
	return 0;
}

std::list<Entity *> *Arena::FindEntityByClass(std::string classname)
{
	std::list<Entity *> *list = 0;

	for (auto& kv : entities)
	{
		Entity *e = kv.second;
		if(classname == e->GetClassname())
		{
			if(!list)
				list = new std::list<Entity *>();

			list->push_back(e);
		}
	}

	return list;
}