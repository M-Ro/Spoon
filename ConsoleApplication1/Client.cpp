#include <iostream>
#include <string>

#include <Windows.h>

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
AudioSystem* audio;

// Called every frame
long lastTime = 0;

void run()
{
	Time::Update();
	FpsCounter::Tick();
	long curTime = Time::GetCurrentTimeMillis();
	long deltaTime = curTime - lastTime;
	lastTime = curTime;

	if (game->gametype != "server") {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0, 0.0, 0.0, 1.0); // Clear screen black
	}

	float dTimef = (float)deltaTime / 1000.0;
	if (dTimef > 0.25) dTimef = 0.25; // Limit  dTime to 0.25s (4fps minimum)
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


	/* Initialise renderer and camera */
	Renderer* renderer = new Renderer();
	Camera* camera = new Camera();
	renderer->SetActiveCamera(camera);

	/* Initialise sound */
	audio = new AudioSystem();

	/* Create the base 'game' */
	game = new GameName();
	game->renderer = renderer;
	game->camera = camera;
	game->input = new InputHandler();
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
			quit = game->input->KeyPressed(SDLK_F12);

		run();
	}

	game->Shutdown();
	delete game;

	delete audio;
	Config::WriteConfig();
	Filesystem::Shutdown();

	return 0;
}