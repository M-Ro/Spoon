#pragma once

#include "../render/Renderer.h"
#include "../input/InputHandler.h"

class Game
{
public:
	// Initialization
	Game() {}

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
	//AudioSystem *audio;
	InputHandler *input;
};