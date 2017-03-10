#include <iostream>
#include <List>

#include <glm/gtc/type_ptr.hpp>

#include "../../../auxiliary/Network.h"
#include "../../../auxiliary/Time.h"
#include "../../definitions.h"
#include "Spawner.h"
#include "Player.h"
#include "Skull.h"
#include "../Arena.h"

extern Player *player;


Spawner::Spawner() : Monster()
{
	modelname = "skull";
	classname = "spawner_1";
	health = 400;
	speed = 10.0f;
	moveType = MovementType::Fly;
	cmodel = CollisionModel(CollisionModel::ModelType::Sphere, 14);
	solid = true;
	spawn_timer = Time::GetCurrentTimeMillis() + 10000 + 500*rand()/RAND_MAX;

	float center_distance = 500.0f;
	float theta = 6.28*rand()/RAND_MAX;
	position = glm::vec3(center_distance*cos(theta), 50.0f, center_distance*sin(theta));
	velocity = normalize(glm::vec3(0,0,0) - position)*speed;
}

void Spawner::SpawnSkulls(int amount){
	while(amount-- > 0){
		Skull * s = new Skull();
		s->position = position;

		float theta = 6.28*rand()/RAND_MAX;
		float r = rand()/RAND_MAX;
		s->velocity = 80.0f * glm::vec3(cos(theta)*r, sin(theta)*r, 0.5+rand()/RAND_MAX);
		arena->AddEntity(s);
	}
};

void Spawner::SendEntity(){
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

void Spawner::Update(float deltaTime)
{
	
	if(hostmodule){
		if(net_nextSendEntity < Time::GetCurrentTimeMillis()){
			SendEntity();
			net_nextSendEntity = Time::GetCurrentTimeMillis() + 50;
		}
	}
	if(!clientmodule){
		//std::cout<<spawn_timer<<" "<<Time::GetCurrentTimeMillis()<<std::endl;
		if(spawn_timer < Time::GetCurrentTimeMillis()){
			SpawnSkulls(6);
			spawn_timer = Time::GetCurrentTimeMillis() + 10000 + 5000*rand()/RAND_MAX;
		}
	}
}

Spawner::~Spawner()
{
	NetworkPackage p;
	p.addInts((int*)&NET_entRemove, 1);
	p.addInts((int*)&id, 1);

	hostmodule->SendAll(p.msg, p.length);
}

void Spawner::Touch(Entity *other)
{
	
}

void Spawner::Hurt(Entity *attacker, float dmg)
{
	if(attacker->team != team)
		Die();
}

void Spawner::Die()
{
	destroy = true;
}