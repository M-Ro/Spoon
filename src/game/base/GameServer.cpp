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
}

void GameServer::AddNewPlayer(IPaddress * address){
	Player * p = new Player();
	p->myAddress = address;
	arena->AddEntity(p);
}