#include "Player.h"
#include "projectiles/Spoon.h"
#include "../../../auxiliary/Config.h"
#include "../../../auxiliary/Network.h"
#include <iostream>
#include <cmath>

Player::Player(InputHandler *input) : Entity()
{
	classname = "player";
	this->input = input;
	team = Team::Player;
	moveType = MovementType::Walk;

	position.x = 0;
	position.y = 60;
	position.z = 224;

	cam_rot.x = 3.14;
	cam_rot.y = -0.3;

	bbox = glm::vec3(24, 64, 24);
	solid = true;
	maxSpeed = 180;
}

Player::~Player()
{

}

void Player::Update(float deltaTime)
{
	ProjectView();
	HandlePlayerInput(deltaTime);
}

void Player::Touch(Entity *other)
{
	//if(other->team == Team::Monster)
		//std::cout << "u dieded" << std::endl;
}

void Player::Hurt(Entity *attacker, float dmg)
{

}

void Player::Die()
{

}

void Player::ProjectView()
{
    dir = glm::vec3(cos(cam_rot.y) * sin(cam_rot.x), 
        sin(cam_rot.y),
        cos(cam_rot.y) * cos(cam_rot.x)
    );

    right = glm::vec3(sin(cam_rot.x - 3.14f/2.0f), 0, cos(cam_rot.x - 3.14f/2.0f));

    up = glm::cross(right, dir);
}

void Player::HandlePlayerInput(float deltaTime)
{
	const float MOVEMENT_SPEED = 1000.0;

	cam_rot.x -= input->GetMouseX() / 100.0;
	cam_rot.y -= input->GetMouseY() / 100.0;
	cam_rot.y = std::fmax(std::fmin(cam_rot.y, 1.4), -1.4);

	glm::vec3 oldVel = velocity;

	/* Calculate movement vector */
	glm::vec3 moveDir = glm::vec3(1 * sin(cam_rot.x), 0, 1 * cos(cam_rot.x));

	if(input->KeyDown(Config::GetKey("moveForward")))
	{
		if(onFloor && moveType == MovementType::Walk)
			velocity += moveDir * deltaTime * MOVEMENT_SPEED;
		else if(moveType == MovementType::Fly)
			position += dir * deltaTime * (MOVEMENT_SPEED/5);
	}
	if(input->KeyDown(Config::GetKey("moveBackward")))
	{
		if(onFloor && moveType == MovementType::Walk)
			velocity -= moveDir * deltaTime * MOVEMENT_SPEED;
		else if(moveType == MovementType::Fly)
			position -= dir * deltaTime * (MOVEMENT_SPEED/5);
	}
	if(input->KeyDown(Config::GetKey("moveLeft")))
	{
		if(onFloor && moveType == MovementType::Walk)
			velocity -= right * deltaTime * MOVEMENT_SPEED;
		else if(moveType == MovementType::Fly)
			position -= right * deltaTime * (MOVEMENT_SPEED/5);
	}
	if(input->KeyDown(Config::GetKey("moveRight")))
	{
		if(onFloor && moveType == MovementType::Walk)
			velocity += right * deltaTime * MOVEMENT_SPEED;
		else if(moveType == MovementType::Fly)
			position += right * deltaTime * (MOVEMENT_SPEED/5);
	}
	if(input->KeyDown(Config::GetKey("jump")))
	{
		if(onFloor && moveType == MovementType::Walk)
		{
			velocity.y = 40;
			onFloor = false;
		}
		else if(moveType == MovementType::Fly)
			position.y += 150 * deltaTime;
	}
	if(input->MousePressed(1)) // FIXME this is ugly and should be written properly
	{
		Spoon *spoon = new Spoon(this);
		spoon->dir = dir;
		spoon->position = position;

		spoon->rotation = glm::vec3(cam_rot.x, cam_rot.y, 0); // idk

		arena->AddEntity(spoon);
	}

	// :D
	if(input->KeyDown(SDLK_LALT) && input->KeyPressed(SDLK_F4))
		moveType = (moveType == MovementType::Walk ? MovementType::Fly : MovementType::Walk);

	if(oldVel != velocity)
	{
		if(glm::clamp(velocity, -maxSpeed, maxSpeed) != velocity)
			velocity = oldVel;
	}
	if(input->KeyDown(SDLK_e))
		if(clientmodule == NULL)
			InitialiseClient("127.0.0.1",44,45);
	if(input->KeyDown(SDLK_q))
		if(hostmodule == NULL)
			InitialiseHost(45);
}