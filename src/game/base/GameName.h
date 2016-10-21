#pragma once

#include "../Game.h"

class GameName : public Game
{
public:
	GameName();

	~GameName();

	// Asset loading / game setup
	void Load();

	void Shutdown();

	void Run(float deltaTime);

};