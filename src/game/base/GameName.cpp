#include "GameName.h"

#include "entity/Player.h"
#include "entity/Skull.h"

Player *player;
Skull * skull;

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
	skull = new Skull();
}

void GameName::Shutdown()
{
	delete player;
	delete skull;
}

void GameName::Run(float deltaTime)
{
	input->Update();

	glm::vec3 pos(0.0, 0.0, 0.0);
	renderer->DrawModel("arena", pos, glm::vec3(0.0f,0.0f,0.0f));
	renderer->DrawModel("skull", skull->position, skull->rotation);
	skull->Update(deltaTime);
	player->Update(deltaTime);

	camera->SetPosition(player->position.x, player->position.y, player->position.z);
	camera->SetAngle(player->cam_rot.x, player->cam_rot.y);
	camera->Update();

	renderer->Flip();
}