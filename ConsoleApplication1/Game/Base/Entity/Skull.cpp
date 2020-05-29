#include <iostream>
#include <List>

#include <glm/gtc/type_ptr.hpp>

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
	moveType = MovementType::Fly;
	cmodel = CollisionModel(CollisionModel::ModelType::Sphere, 14);
	solid = true;
	enemy = NULL;
	//	FIXME should sync these once for mp
	phase = (float)rand();
	speed = 80.0f + 20.0f*rand()/RAND_MAX;
	swayspeed = 0.1256f + 0.2f*rand()/RAND_MAX;
	swaystrength = 0.2f + 0.1f*rand()/RAND_MAX;
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

	if(deltaTime > 1.0f)
		deltaTime = 1.0f;
	glm::vec3 enemy_direction = glm::normalize(enemy->position - position);
	
	float temp = turnrate/glm::length(enemy_direction);
	velocity = 0.999f*velocity + glm::vec3(temp*enemy_direction[0], temp*enemy_direction[1], temp*enemy_direction[2]);

	if(glm::length(velocity) > speed)
		velocity = speed * glm::normalize(velocity);

	glm::vec3 phase_direction = cos(phase) * glm::normalize(glm::cross(enemy_direction, glm::vec3(0,1,0))) + sin(phase) * glm::vec3(0,1,0);
	velocity += phase_direction * swaystrength;
	phase += deltaTime*swayspeed;
	
	//	To keep skulls from sinking to the floor
	if(velocity[1] < 0.0f)
		velocity[1] = velocity[1] * (1-exp(-fabs(position[1])*0.005f));

	float angle = atan(velocity[0]/velocity[2]);
	if(velocity[2] < 0.0f)
		angle += 3.14f;

	rotation[1] = angle;

	angle = asin(velocity[1]/glm::length(velocity));
	rotation[0] = -angle;

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