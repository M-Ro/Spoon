#pragma once
#include <string>

#include "Globals.h"
#include "../Physics/Physics.h"

class Game
{
public:
	// Initialization
	Game() {
		renderer = nullptr;
		camera = nullptr;
		input = nullptr;
		audio = nullptr;

		Physics::Initialise();
	}

	// Asset loading / game setup
	virtual void Load() = 0;

	// Final cleanup
	virtual ~Game() {
		Physics::Shutdown();
	};

	// Cleanup, called before destructor
	virtual void Shutdown() = 0;

	// Called every frame
	virtual void Run(float deltaTime) = 0;

	std::string gametype;
};

extern Game* game;