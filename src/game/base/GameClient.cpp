#include <sstream>
#include <vector>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "GameClient.h"

#include "Arena.h"
#include "entity/Player.h"
#include "entity/Skull.h"
#include "entity/projectiles/Spoon.h"

#include "../../auxiliary/fpscounter.h"
#include "../../auxiliary/Network.h"

extern Arena *arena;
extern Player *player;

GameClient::GameClient()
{
	game = this;
	gametype = "client";
}

GameClient::~GameClient()
{

}

void GameClient::Load()
{
	input->SetRelativeMouseMode(true);

	renderer->PrecacheModel("arena");
	renderer->PrecacheModel("skull");
	renderer->PrecacheModel("spoon");

	arena = new Arena("arena");

	player = new Player();
	player->input = input;
	arena->AddEntity(player);
}

void GameClient::Shutdown()
{
	delete player;
}

void GameClient::Run(float deltaTime)
{
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

	clientmodule->CheckForData();
}

void GameClient::HandleNetworkMsg(char * data){
	int id, name_len;
	memcpy(&id, data, sizeof(int));
	memcpy(&name_len, data + sizeof(int), sizeof(int));
	std::string classname(data + 2*sizeof(int),data + 2*sizeof(int) + name_len);

	Entity * e = arena->FindEntityById(id);
	int offset = sizeof(int)*2 + name_len;
	if(e != 0){
		for(int i = 0; i < 3; i++){
			memcpy(glm::value_ptr(e->position), data + offset, 						sizeof(float)*3);
			memcpy(glm::value_ptr(e->velocity), data + offset + sizeof(float)*3,	sizeof(float)*3);
			memcpy(glm::value_ptr(e->rotation), data + offset + sizeof(float)*6,	sizeof(float)*3);
		}
	}
	else if(classname == "skull"){
		Skull * s = new Skull();
		arena->AddEntity(s);
		s->id = id;
		return;	//	FIXME: it will crash without this though
		for(int i = 0; i < 3; i++){
			memcpy(glm::value_ptr(e->position), data + offset, 						sizeof(float)*3);
			memcpy(glm::value_ptr(e->velocity), data + offset + sizeof(float)*3,	sizeof(float)*3);
			memcpy(glm::value_ptr(e->rotation), data + offset + sizeof(float)*6,	sizeof(float)*3);
		}
	}
	else if(classname == "projectile_spoon"){
		Spoon * s = new Spoon(player);
		arena->AddEntity(s);
		s->id = id;
		return;	//	FIXME: it will crash without this though
		for(int i = 0; i < 3; i++){
			memcpy(glm::value_ptr(e->position), data + offset, 						sizeof(float)*3);
			memcpy(glm::value_ptr(e->velocity), data + offset + sizeof(float)*3,	sizeof(float)*3);
			memcpy(glm::value_ptr(e->rotation), data + offset + sizeof(float)*6,	sizeof(float)*3);
		}
	}
}