#ifdef _DEBUG
#include <stdlib.h>
#include <crtdbg.h>
#define _CRTDBG_MAP_ALLOC
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

#include <iostream>
#include <string>

#ifdef _WIN32
	#include <Windows.h>
#endif

#include <SDL2/SDL.h>
#include <GL\glew.h>

#include "auxiliary/Time.h"
#include "auxiliary/Config.h"
#include "auxiliary/fpscounter.h"
#include "auxiliary/Filesystem.h"
#include "render/Renderer.h"
#include "render/Camera.h"
#include "audio/AudioSystem.h"
#include "game/base/GameName.h"
#include "input/InputHandler.h"

bool quit = false;

Game* game;

// Called every frame
long long lastTime = 0;

void run()
{
	Time::Update();
	FpsCounter::Tick();
	long long curTime = Time::GetCurrentTimeMillis();
	long long deltaTime = curTime - lastTime;
	lastTime = curTime;

	if (game->gametype != "server") {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 1.0); // Clear screen black
	}

	float dTimef = (float)deltaTime / 1000.0f;
	if (dTimef > 0.25f) dTimef = 0.25f; // Limit  dTime to 0.25s (4fps minimum)
	game->Run(dTimef);
}

bool startSingle() {
	/* Init SDL */
	std::cout << "Initialising SDL" << std::endl;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) // Initialise SDL
	{
		std::cout << "Error: Could not initialise SDL: " << SDL_GetError() << std::endl;
		return 0;
	}


	/* Create the base 'game' */
	game = new GameName();
	renderer = new Renderer();
	camera = new Camera();
	input = new InputHandler();

	renderer->SetActiveCamera(camera);

	return 1;
}

int main(int argc, char** argv)
{
	Time::Initialise();
	Filesystem::Initialise(argv[0]);

	std::cout << "Starting singeplayer..." << std::endl;
	startSingle();

	game->Load();
	Time::Update(); // First frame

	while (!quit)
	{
		// fixme dirty hack
		if (game->gametype != "server")
			quit = input->KeyPressed(SDLK_F11);

		run();
	}

	game->Shutdown();
	delete game;

	Config::WriteConfig();
	Filesystem::Shutdown();
	SDL_Quit();

#ifdef _DEBUG
#ifdef _WIN32
	_CrtDumpMemoryLeaks();
#endif
#endif

	return 0;
}