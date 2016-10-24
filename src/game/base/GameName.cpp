#include <sstream>

#include "GameName.h"

#include "Arena.h"
#include "entity/Player.h"
#include "entity/Skull.h"

#include "../../auxiliary/fpscounter.h"
#include "../../auxiliary/Network.h"

Arena *arena;
Player *player;

GameName::GameName()
{
	game = this;
}

GameName::~GameName()
{

}

void GameName::Load()
{
	input->SetRelativeMouseMode(true);

	arena = new Arena("arena");

	player = new Player(input);

	arena->AddEntity(player);
	arena->AddEntity(new Skull());
}

void GameName::Shutdown()
{
	delete player;
}
float hitimer = 0.0f;

void GameName::Run(float deltaTime)
{
	if(hostmodule != NULL){
		hostmodule->CheckForData();
		hitimer -= deltaTime;
		if(hitimer < 0.0f){
			hitimer += 2.0f;
			hostmodule->SendAll("Wow");
		}
	}

	if(clientmodule != NULL){
		clientmodule->CheckForData();
	}

	input->Update();

	arena->Update(deltaTime);
	arena->Draw();

	camera->SetPosition(player->position.x, player->position.y, player->position.z);
	camera->SetAngle(player->cam_rot.x, player->cam_rot.y);
	camera->Update();

	std::stringstream ss;
	ss << "FPS: " << FpsCounter::fps;

	renderer->DrawText(ss.str(), -0.99, 0.95);

	renderer->Flip();
}