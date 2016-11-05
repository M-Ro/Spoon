#include <iostream>
#include <List>

#include <glm/gtc/type_ptr.hpp>

#include "../../../auxiliary/Network.h"
#include "../../../auxiliary/Time.h"
#include "../../definitions.h"
#include "Skull.h"
#include "Player.h"

extern Player *player;

Skull::Skull() : Monster()
{
	modelname = "skull";
	classname = "skull";
	health = 4;
	position = glm::vec3(0.0f, 20.0f, 00.0f);
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	turnrate = 0.9f;
	speed = 80.0f;
	moveType = MovementType::Fly;
	cmodel = CollisionModel(CollisionModel::ModelType::Sphere, 14);
	solid = true;
	enemy = NULL;
}

void Skull::SendEntity(){
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

void Skull::Update(float deltaTime)
{
	//	Find new enemy, if we don't have one
	if(!enemy){
		std::list<Entity *> * playerlist = arena->FindEntityByClass("player");
		if(playerlist == 0)
			return;
		else
			enemy = playerlist->front();
		delete playerlist;
	}

	if(deltaTime > 1.0)
		deltaTime = 1.0;
	glm::vec3 enemy_direction = glm::normalize(enemy->position - position);
	
	float temp = turnrate/glm::length(enemy_direction);
	velocity = 0.999f*velocity + glm::vec3(temp*enemy_direction[0], temp*enemy_direction[1], temp*enemy_direction[2]);

	if(glm::length(velocity) > speed)
		velocity = speed * glm::normalize(velocity);

	//position += velocity*deltaTime;
	
	//velocity = enemy_direction*speed;
	//position += velocity;



	//velocity = enemy_direction;
	
	float angle = atan(velocity[0]/velocity[2]);
	if(velocity[2] < 0.0f)
		angle += 3.14f;

	rotation[1] = angle;

	angle = asin(velocity[1]/glm::length(velocity));
	rotation[0] = -angle;

	if(hostmodule){
		if(net_nextSendEntity < Time::GetCurrentTimeMillis()){
			SendEntity();
			net_nextSendEntity = Time::GetCurrentTimeMillis() + 50;
		}
	}

	return;
}

Skull::~Skull()
{

}

void Skull::Touch(Entity *other)
{
	
}

void Skull::Hurt(Entity *attacker, float dmg)
{
	if(attacker->team != team)
		Die();
}

void Skull::Die()
{
	destroy = true;
}