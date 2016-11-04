#pragma once

#include "../Game.h"
#include <string>

class GameClient : public Game
{
public:
	GameClient();

	~GameClient();

	// Asset loading / game setup
	void Load();

	void Shutdown();

	void Run(float deltaTime);

	void HandleNetworkMsg(char * data);
	void AddNewPlayer(IPaddress * address){};
};