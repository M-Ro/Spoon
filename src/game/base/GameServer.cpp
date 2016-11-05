#include <sstream>
#include <vector>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "GameServer.h"

#include "Arena.h"
#include "entity/Player.h"
#include "entity/Skull.h"
#include "entity/Entity.h"
#include "entity/projectiles/Spoon.h"

#include "../../auxiliary/fpscounter.h"
#include "../../auxiliary/Network.h"
#include "../definitions.h"

extern Arena *arena;

GameServer::GameServer()
{
	game = this;
	gametype = "server";
}

GameServer::~GameServer()
{

}

void GameServer::Load()
{
	arena = new Arena("arena");
	arena->AddEntity(new Skull());
}

void GameServer::Shutdown()
{
	delete arena;
}

void GameServer::Run(float deltaTime)
{
	hostmodule->CheckForData();
	arena->Update(deltaTime);
}

void GameServer::HandleNetworkMsg(char * data){
	int packet_type;
	memcpy(&packet_type, data, sizeof(int));
	//std::cout<<packet_type<<std::endl;
	if(packet_type == NET_selfInfo){
		unsigned int id;
		memcpy(&id, data+sizeof(int), sizeof(int));

		Entity * e = arena->FindEntityById(id);
		int offset = sizeof(int)*2;
		if(e != 0){
			e->net_lastUpdate = Time::GetCurrentTimeMillis();
			for(int i = 0; i < 3; i++){
				memcpy(glm::value_ptr(e->position), data + offset, 						sizeof(float)*3);
				memcpy(glm::value_ptr(e->velocity), data + offset + sizeof(float)*3,	sizeof(float)*3);
				memcpy(glm::value_ptr(e->rotation), data + offset + sizeof(float)*6,	sizeof(float)*3);
			}
		}
	}
}

void GameServer::AddNewPlayer(IPaddress * address){
	Player * p = new Player();
	p->myAddress = address;
	arena->AddEntity(p);
}