#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "Spoon.h"
#include "../../../../auxiliary/Network.h"
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

void Spoon::SendEntity(){
	NetworkPackage p;
	int name_len = classname.size();
	p.addInts((int*)&NET_entUpdate, 1);
	p.addInts((int*)&id, 1);
	p.addInts((int*)&name_len, 1);
	p.addString(&classname);
	p.addFloats(glm::value_ptr(position), 3);
	p.addFloats(glm::value_ptr(velocity), 3);
	p.addFloats(glm::value_ptr(rotation), 3);

	hostmodule->SendAll(p.msg, p.length);
}

void Spoon::Update(float deltaTime)
{
	if(Time::GetCurrentTimeMillis() > spawntime + lifetime)
		Die();

	velocity = dir * speed;

	position += velocity * deltaTime;

	if(hostmodule){
		if(net_nextSendEntity < Time::GetCurrentTimeMillis()){
			SendEntity();
			net_nextSendEntity = Time::GetCurrentTimeMillis() + 50;
		}
	}
	return;
}

Spoon::~Spoon()
{
	NetworkPackage p;
	p.addInts((int*)&NET_entRemove, 1);
	p.addInts((int*)&id, 1);

	hostmodule->SendAll(p.msg, p.length);
}