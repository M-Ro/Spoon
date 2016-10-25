#include "Player.h"
#include "../../../auxiliary/Config.h"
#include "../../../auxiliary/Network.h"
#include <iostream>

Player::Player(InputHandler *input) : Entity()
{
	classname = "player";
	this->input = input;
	team = Team::Player;

	position.x = 0;
	position.y = 60;
	position.z = 250;

	cam_rot.x = 3.14;
	cam_rot.y = -0.3;
}

Player::~Player()
{

}

void Player::Update(float deltaTime)
{
	ProjectView();
	HandlePlayerInput(deltaTime);
}

void Player::Hurt(float dmg)
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
	cam_rot.x -= input->GetMouseX() / 100.0;
	cam_rot.y -= input->GetMouseY() / 100.0;

	if(input->KeyDown(Config::GetKey("moveForward")))
		position += dir * deltaTime * MOVEMENT_SPEED;
	if(input->KeyDown(Config::GetKey("moveBackward")))
		position -= dir * deltaTime * MOVEMENT_SPEED;
	if(input->KeyDown(Config::GetKey("moveLeft")))
		position -= right * deltaTime * MOVEMENT_SPEED;
	if(input->KeyDown(Config::GetKey("moveRight")))
		position += right * deltaTime * MOVEMENT_SPEED;
	if(input->KeyDown(Config::GetKey("jump")))
		position += up * deltaTime * MOVEMENT_SPEED;
	if(input->KeyDown(SDLK_e))
		if(clientmodule == NULL)
			InitialiseClient("127.0.0.1",44,45);
	if(input->KeyDown(SDLK_q))
		if(hostmodule == NULL)
			InitialiseHost(45);
}