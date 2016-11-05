#include <sstream>
#include <vector>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "GameClient.h"

#include "Arena.h"
#include "entity/Player.h"
#include "entity/Skull.h"
#include "entity/projectiles/Spoon.h"

#include "../../auxiliary/Time.h"
#include "../../auxiliary/fpscounter.h"
#include "../../auxiliary/Network.h"
#include "../definitions.h"

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
	if(player->net_nextSendEntity < Time::GetCurrentTimeMillis()){
		SendStatus();
		player->net_nextSendEntity = Time::GetCurrentTimeMillis() + 50;
	}
}

void GameClient::EntityUpdate(char * data){
	unsigned int id, name_len;
	memcpy(&id, data, sizeof(int));

	if(id == player->id)
		return;

	memcpy(&name_len, data + sizeof(int), sizeof(int));
	std::string classname(data + 2*sizeof(int),data + 2*sizeof(int) + name_len);
	//std::cout<<id<<" "<<classname<<std::endl;
	Entity * e = arena->FindEntityById(id);
	int offset = sizeof(int)*2 + name_len;
	if(e != 0){
		e->net_lastUpdate = Time::GetCurrentTimeMillis();
		for(int i = 0; i < 3; i++){
			memcpy(glm::value_ptr(e->position), data + offset, 						sizeof(float)*3);
			memcpy(glm::value_ptr(e->velocity), data + offset + sizeof(float)*3,	sizeof(float)*3);
			memcpy(glm::value_ptr(e->rotation), data + offset + sizeof(float)*6,	sizeof(float)*3);
		}
	}
	else if(classname == "skull"){
		Skull * s = new Skull();
		s->id = id;
		arena->AddEntity(s);
		return;	//	FIXME: it will crash without this though
		for(int i = 0; i < 3; i++){
			memcpy(glm::value_ptr(e->position), data + offset, 						sizeof(float)*3);
			memcpy(glm::value_ptr(e->velocity), data + offset + sizeof(float)*3,	sizeof(float)*3);
			memcpy(glm::value_ptr(e->rotation), data + offset + sizeof(float)*6,	sizeof(float)*3);
		}
	}
	else if(classname == "projectile_spoon"){
		Spoon * s = new Spoon(player);
		s->id = id;
		arena->AddEntity(s);
		return;	//	FIXME: it will crash without this though
		for(int i = 0; i < 3; i++){
			memcpy(glm::value_ptr(e->position), data + offset, 						sizeof(float)*3);
			memcpy(glm::value_ptr(e->velocity), data + offset + sizeof(float)*3,	sizeof(float)*3);
			memcpy(glm::value_ptr(e->rotation), data + offset + sizeof(float)*6,	sizeof(float)*3);
		}
	}
	else if(classname == "player"){
		Spoon * s = new Spoon(player);
		s->id = id;
		arena->AddEntity(s);
		return;	//	FIXME: it will crash without this though
		for(int i = 0; i < 3; i++){
			memcpy(glm::value_ptr(e->position), data + offset, 						sizeof(float)*3);
			memcpy(glm::value_ptr(e->velocity), data + offset + sizeof(float)*3,	sizeof(float)*3);
			memcpy(glm::value_ptr(e->rotation), data + offset + sizeof(float)*6,	sizeof(float)*3);
		}
	}
}

void GameClient::SelfUpdate(char * data){
	int id;
	memcpy(&id, data, sizeof(int));
	player->id = id;
	if(!player->s_knowsId){
		arena->AddEntity(player);
		player->s_knowsId = true;
	}
}

void GameClient::HandleNetworkMsg(char * data){
	int packet_type;
	memcpy(&packet_type, data, sizeof(int));
	//std::cout<<packet_type<<std::endl;
	if(packet_type == NET_entUpdate)
		EntityUpdate(data + sizeof(int));
	if(packet_type == NET_selfInfo)
		SelfUpdate(data + sizeof(int));
	
}

void GameClient::SendStatus(){
	int package_size = sizeof(int)*2 + 9*sizeof(float);

	char * package = new char[package_size];
	size_t loc = (size_t)package;
	loc = 0;

	memcpy(package, &NET_selfInfo, sizeof(int));
	loc += sizeof(int);
	memcpy(package+loc, &player->id, sizeof(int));
	loc += sizeof(int);
	memcpy(package+loc, glm::value_ptr(player->position), sizeof(float)*3);
	loc += sizeof(float)*3;
	memcpy(package+loc, glm::value_ptr(player->velocity), sizeof(float)*3);
	loc += sizeof(float)*3;
	memcpy(package+loc, glm::value_ptr(player->rotation), sizeof(float)*3);

	clientmodule->Send(package, package_size);

	delete [] package;
}