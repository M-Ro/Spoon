#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "Spoon.h"
#include "../../../../auxiliary/Network.h"
#include "../../../../auxiliary/Time.h"

Spoon::Spoon(Entity *owner) : Projectile(owner)
{
	classname = "projectile_spoon";
	modelname = "spoon";
	bbox = glm::vec3(6, 6, 6);
	lifetime = 4000;
	speed = 500;
	solid = true;
}

void Spoon::SendEntity(){
	int name_len = classname.size();
	int package_size = sizeof(int)*2 + 9*sizeof(float) + name_len;

	char * package = new char[package_size];
	size_t loc = (size_t)package;
	loc = 0;

	memcpy(package, &id, sizeof(int));
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
	nextupdate = 0.05;
}

void Spoon::Update(float deltaTime)
{
	if(Time::GetCurrentTimeMillis() > spawntime + lifetime)
		Die();

	velocity = dir * speed;

	position += velocity * deltaTime;

	nextupdate -=deltaTime;
	if(hostmodule && nextupdate < 0.0f)
		SendEntity();
	return;
}

Spoon::~Spoon()
{

}