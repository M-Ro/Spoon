#include "Player.h"
#include "projectiles/Spoon.h"
#include "../../../auxiliary/Config.h"
#include "../../../auxiliary/Network.h"
#include "../../../auxiliary/Time.h"
#include "../../definitions.h"
#include <iostream>
#include <cmath>

Player::Player() : Entity()
{
	classname = "player";
	team = Team::Player;
	moveType = MovementType::Walk;

	position.x = 0;
	position.y = 60;
	position.z = 224;

	cam_rot.x = 3.14;
	cam_rot.y = -0.3;

	cmodel = CollisionModel(CollisionModel::ModelType::Box, glm::vec3(24, 64, 24));
	solid = true;

	lastJumpTime = 0;
}

Player::~Player()
{

}

void Player::SendEntity(){
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

void Player::Update(float deltaTime)
{
	if(hostmodule){
		if(net_nextSendEntity < Time::GetCurrentTimeMillis()){
			SendEntity();
			net_nextSendEntity = Time::GetCurrentTimeMillis() + 50;
		}
		return;
	}
	ProjectView();
	HandlePlayerInput(deltaTime);
	HandlePMove(deltaTime);

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
	cam_rot.x -= input->GetMouseX() / 300.0;
	cam_rot.y -= input->GetMouseY() / 300.0;
	cam_rot.y = std::fmax(std::fmin(cam_rot.y, 1.4), -1.4);

	idir = glm::vec3(0, 0, 0);

	glm::vec3 moveDir = glm::vec3(sin(cam_rot.x), 0, cos(cam_rot.x));
	if(moveType == MovementType::Fly)
		moveDir = dir;

	if(input->KeyDown(Config::GetKey("moveForward")))
		idir += moveDir;
	else if(input->KeyDown(Config::GetKey("moveBackward")))
		idir -= moveDir;

	if(input->KeyDown(Config::GetKey("moveLeft")))
		idir -= right;
	else if(input->KeyDown(Config::GetKey("moveRight")))
		idir += right;

	if(input->KeyDown(Config::GetKey("jump")))
		idir.y = 1;

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
	{
		moveType = (moveType == MovementType::Walk ? MovementType::Fly : MovementType::Walk);
		velocity = glm::vec3(0, 0, 0);
	}

	if(input->KeyPressed(SDLK_b))
		arena->SetDrawBBoxes();
}

void Player::HandlePMove(float deltaTime)
{
	const float ACCEL_SPEED = 1800;
	const float AIR_C = 200;
	const float MAX_SPEED = 250;
	const float MAX_AIR_SPEED = 300;
	const float JUMP_HEIGHT = 80;
	const float gravity = 180;
	const float airFriction = 50;
	const float friction = 1000;
	const float termVel = 210;

	onFloor = cmodel.OnFloor(position, arena->GetCModel());

	/* Calculate intended movement direction */
	glm::vec3 idirxzn = glm::vec3(0, 0, 0);
	if(glm::length(glm::vec3(idir.x, 0, idir.z)) > 0)
		idirxzn = glm::vec3(idir.x, 0, idir.z);

	if(moveType == MovementType::Walk)
	{
		glm::vec3 xzVel = glm::vec3(velocity.x, 0, velocity.z);
		glm::vec3 xzVelN = glm::vec3(0, 0, 0);
		if(glm::length(xzVel) != 0)
			xzVelN = glm::normalize(xzVel);

		/* If airbound, apply airFriction and gravity */
		if(!onFloor)
		{
			/* Air friction, gravity */
			velocity -= (deltaTime * xzVelN * airFriction);
			velocity.y -= deltaTime * gravity;

			if(velocity.y < -termVel)
				velocity.y = -termVel;

			/* Air Control */
			velocity += (idirxzn * AIR_C * deltaTime);

			// Clamp speed
			if(glm::length(velocity) > MAX_AIR_SPEED)
				velocity = glm::normalize(velocity) * MAX_AIR_SPEED; // todo don't hardclamp, just add more friction

			/* Terrible hack to 'pop' the player off floor */
			if(cmodel.OnFloor(position, arena->GetCModel()))
			{
				while(cmodel.OnFloor(position, arena->GetCModel()))
					position.y += 1;

				position.y -= 1;
				velocity.y = 0;
			}
		}
		else // On Floor
		{
			if(velocity.y < 0) // Stop ent going through floor
				velocity.y = 0;

			if(glm::length(xzVelN) > 0 && glm::dot(idirxzn, xzVelN) < 0)
				velocity -= deltaTime * xzVelN * (friction*2);

			velocity -= (deltaTime * xzVelN * friction);
			velocity += (idirxzn * ACCEL_SPEED * deltaTime);

			// Clamp speed
			if(glm::length(velocity) > MAX_SPEED)
				velocity = glm::normalize(velocity) * MAX_SPEED;

			// Apply vertical velocity (jumping)
			if(idir.y > 0 && Time::GetCurrentTimeMillis() > lastJumpTime + 300)
			{
				lastJumpTime = Time::GetCurrentTimeMillis();
				velocity.y = idir.y * JUMP_HEIGHT;
				onFloor = false;
			}
		}
	}
	else if(moveType == MovementType::Fly)
	{
		const float flySpeed = 5000;
		if(glm::length(idir) > 0)
			velocity += glm::normalize(idir) * flySpeed * deltaTime;

		if(glm::length(velocity) > 0)
			velocity -= (deltaTime * glm::normalize(velocity) * friction);

		if(glm::length(velocity) > (MAX_AIR_SPEED*2))
			velocity = glm::normalize(velocity) * (MAX_AIR_SPEED*2);
	}

	/* Apply velocity to position */
	position += velocity * deltaTime;
}