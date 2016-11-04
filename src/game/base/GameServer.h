#pragma once

#include "../Game.h"
#include <string>

class GameServer : public Game
{
public:
	GameServer();

	~GameServer();

	// Asset loading / game setup
	void Load();

	void Shutdown();

	void Run(float deltaTime);

	void HandleNetworkMsg(char * data);
	void AddNewPlayer(IPaddress * address);
};