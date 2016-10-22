#include "Skull.h"
#include "Player.h"

extern Player *player;

Skull::Skull() 
{
	health = 4;
	position = glm::vec3(0.0f, 20.0f, 00.0f);
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	turnrate = 0.05f;
	speed = 0.6f;
}

void Skull::Update(float deltaTime)
{

	glm::vec3 enemy_direction = glm::normalize(player->position - position);
	/*
	float temp = turnrate/glm::length(enemy_direction);
	velocity += glm::vec3(temp*enemy_direction[0], temp*enemy_direction[1], temp*enemy_direction[2]);
	position += velocity*deltaTime;
	*/
	velocity = enemy_direction*speed;
	position += velocity;

	float angle = atan(enemy_direction[0]/enemy_direction[2]);
	if(enemy_direction[2] < 0.0f)
		angle = 3.14f + angle;

	rotation[1] = angle;
	rotation[2] = 0.0f;
	rotation[0] = 0.0f;
}

Skull::~Skull()
{

}

void Skull::Hurt(float dmg)
{

}

void Skull::Die()
{

}