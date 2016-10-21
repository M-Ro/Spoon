#include "GameName.h"

#include "entity/Player.h"

Player *player;

GameName::GameName()
{

}

GameName::~GameName()
{

}

void GameName::Load()
{
	input->SetRelativeMouseMode(true);

	player = new Player(input);
}

void GameName::Shutdown()
{
	delete player;
}

void GameName::Run(float deltaTime)
{
	input->Update();

	glm::vec3 pos(0.0, 0.0, 0.0);
	renderer->DrawModel("arena", pos);
	pos.y = 20;
	renderer->DrawModel("skull", pos);

	player->Update(deltaTime);

	camera->SetPosition(player->position.x, player->position.y, player->position.z);
	camera->SetAngle(player->cam_rot.x, player->cam_rot.y);
	camera->Update();

	renderer->Flip();
}