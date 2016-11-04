#pragma once
#include <string>
#include <SDL2/SDL_net.h>

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
	virtual void HandleNetworkMsg(char * data) = 0;
	virtual void AddNewPlayer(IPaddress * address) = 0;

	Renderer *renderer;
	Camera *camera;
	//Config *config;
	//AudioSystem *audio;
	InputHandler *input;
	std::string gametype;
};

extern Game* game;