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
	int name_len = classname.size();
	int package_size = sizeof(int)*3 + 9*sizeof(float) + name_len;

	char * package = new char[package_size];
	size_t loc = (size_t)package;
	loc = 0;

	memcpy(package, &NET_entUpdate, sizeof(int));
	loc += sizeof(int);
	memcpy(package+loc, &id, sizeof(int));
	loc += sizeof(int);
	memcpy(package+loc, &name_len, sizeof(int));
	loc += sizeof(int);
	memcpy(package+loc, classname.c_str(), name_len);
	loc += name_len;
	memcpy(package+loc, glm::value_ptr(position), sizeof(float)*3);
	loc += sizeof(float)*3;
	memcpy(package+loc, glm::value_ptr(velocity), sizeof(float)*3);
	loc += sizeof(float)*3;
	memcpy(package+loc, glm::value_ptr(rotation), sizeof(float)*3);

	hostmodule->SendAll(package, package_size);
	delete [] package;
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

}