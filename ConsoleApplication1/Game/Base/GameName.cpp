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

//#include "../../Render/Mapfile.h"

Arena *arena;
Player *player;

GameName::GameName() : Game()
{
	game = this;
	gametype = "single";
}

GameName::~GameName()
{
	if (this->camera) {
		delete this->camera;
	}

	if (this->renderer) {
		delete this->renderer;
	}

	if (this->input) {
		delete this->input;
	}
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

	//Mapfile map("e1m1.map");
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

	camera->SetPosition(player->position.x, player->position.y, player->position.z);
	camera->SetAngle(player->cam_rot.x, player->cam_rot.y);
	camera->Update();

	std::stringstream ss;
	ss << "FPS: " << FpsCounter::fps;

	renderer->DrawText(ss.str(), -0.99f, 0.95f);

	glm::vec3 zero(0, 0, 0);
	//renderer->DrawModel("balls", zero, zero);

	renderer->Flip();
}