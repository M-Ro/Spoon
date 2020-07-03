#pragma once
#include <string>

#include "../render/Renderer.h"
#include "../input/InputHandler.h"
#include "AudioInterface.h"

#include "Base/Entity/Entity.h"

class Game
{
public:
	// Initialization
	Game() {
		this->renderer = nullptr;
		this->camera = nullptr;
		this->input = nullptr;
		this->audio = nullptr;
	}

	// Asset loading / game setup
	virtual void Load() = 0;

	// Final cleanup
	virtual ~Game() {};

	// Cleanup, called before destructor
	virtual void Shutdown() = 0;

	// Called every frame
	virtual void Run(float deltaTime) = 0;

	Renderer *renderer;
	Camera *camera;
	//Config *config;
	AudioInterface *audio;
	InputHandler *input;
	std::string gametype;
};

extern Game* game;