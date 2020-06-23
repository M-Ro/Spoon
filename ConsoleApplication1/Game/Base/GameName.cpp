#include <sstream>
#include <vector>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "GameName.h"

#include "Arena.h"
#include "entity/Player.h"
#include "entity/Skull.h"
#include "entity/projectiles/Spoon.h"

#include "../../auxiliary/fpscounter.h"

Arena *arena;
Player *player;

GameName::GameName()
{
	game = this;
	gametype = "single";
}

GameName::~GameName()
{

}

void GameName::Load()
{
	input->SetRelativeMouseMode(true);

	renderer->PrecacheModel("arena");
	renderer->PrecacheModel("skull");
	renderer->PrecacheModel("spoon");

	arena = new Arena("arena");

	player = new Player();
	player->input = input;

	arena->AddEntity(player);
	arena->AddEntity(new Skull());
}

void GameName::Shutdown()
{
	delete player;
}

void GameName::Run(float deltaTime)
{
	input->Update();

	arena->Update(deltaTime);
	arena->Draw();

	glm::vec3 recoil = player->weapon->GetRecoil();
	camera->SetPosition(player->position.x + recoil.x, player->position.y + recoil.y, player->position.z + +recoil.z);

	glm::vec3 angle = player->weapon->GetRecoilAngle(&player->cam_rot);
	camera->SetAngle(angle.x, angle.y);
	camera->Update();

	std::stringstream ss;
	ss << "FPS: " << FpsCounter::fps;

	renderer->DrawText(ss.str(), -0.99f, 0.95f);

	renderer->Flip();
}