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

#include "../../Render/Mapfile.h"

#include "EntityMapper.h"

#include "../../Physics/MapTriMeshCooker.h"

Arena *arena;
Player *player; // Free;d later when arena purges entities

GameName::GameName() : Game()
{
	game = this;
	gametype = "single";

	audio = new AudioInterface();
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

	if (this->audio) {
		delete this->audio;
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

	Mapfile map("textest.map");
	MapTriMeshCooker::BuildMapPhysicsMesh(map);
}

void GameName::Shutdown()
{
	delete arena;
}

void GameName::Run(float deltaTime)
{
	Physics::RunStep(deltaTime);
	input->Update();
	audio->Update();

	arena->Update(deltaTime);
	arena->Draw();

	player->weapon->GetRecoilPos(&player->positionRecoiled);
	camera->SetPosition(player->positionRecoiled.x, player->positionRecoiled.y, player->positionRecoiled.z);

	player->weapon->GetRecoilAngle(&player->cam_rot, &player->rotationRecoiled);
	camera->SetAngle(player->rotationRecoiled.x, player->rotationRecoiled.y);
	camera->Update();

	std::stringstream ss;
	ss << "FPS: " << FpsCounter::fps;

	renderer->DrawText(ss.str(), -0.99f, 0.95f);

	glm::vec3 zero(0, 0, 0);
	renderer->DrawModel("balls", zero, zero);

	renderer->Flip();
}