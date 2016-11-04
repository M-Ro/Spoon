#pragma once

#include "../Game.h"
#include <string>

class GameName : public Game
{
public:
	GameName();

	~GameName();

	// Asset loading / game setup
	void Load();

	void Shutdown();

	void Run(float deltaTime);

	void HandleNetworkMsg(char * data){};
	void AddNewPlayer(IPaddress * address){};
};