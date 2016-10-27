#include "Skull.h"
#include "Player.h"

extern Player *player;

Skull::Skull() : Monster()
{
	modelname = "skull";
	bbox = glm::vec3(32, 32, 32);
	health = 4;
	position = glm::vec3(0.0f, 20.0f, 00.0f);
	velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	turnrate = 0.9f;
	speed = 80.0f;
	moveType = MovementType::Fly;
	solid = true;
}

void Skull::Update(float deltaTime)
{
	if(deltaTime > 1.0)
		deltaTime = 1.0;
	glm::vec3 enemy_direction = glm::normalize(player->position - position);
	
	float temp = turnrate/glm::length(enemy_direction);
	velocity = 0.999f*velocity + glm::vec3(temp*enemy_direction[0], temp*enemy_direction[1], temp*enemy_direction[2]);

	if(glm::length(velocity) > speed)
		velocity = speed * glm::normalize(velocity);

	position += velocity*deltaTime;
	
	//velocity = enemy_direction*speed;
	//position += velocity;



	//velocity = enemy_direction;
	
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

void Skull::Hurt(float dmg)
{

}

void Skull::Die()
{

}