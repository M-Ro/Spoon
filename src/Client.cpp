#include <iostream>

#include <SDL2/SDL.h>
#include <GL\glew.h>

#include "auxiliary/Time.h"
#include "auxiliary/fpscounter.h"
#include "auxiliary/Filesystem.h"
#include "render/Renderer.h"
#include "render/Camera.h"
#include "game/base/GameName.h"
#include "input/InputHandler.h"

bool quit = false;

Game *game;

// Called every frame
long lastTime = 0;

void run()
{
	Time::Update();
	FpsCounter::Tick();
	long curTime = Time::GetCurrentTimeMillis();
	long deltaTime = curTime - lastTime;
	lastTime = curTime;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0); // Clear screen black

	game->Run((float)(deltaTime) / 1000.0);
}

int main(int argc, char **argv)
{
	Time::Initialise();
	Filesystem::Initialise(argv[0]);

	/* Init SDL */
	std::cout << "Initialising SDL" << std::endl;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) // Initialise SDL
	{
		std::cout << "Error: Could not initialise SDL: " << SDL_GetError() << std::endl;
		return 1;
	}

	/* Initialise renderer and camera */
	Renderer renderer = Renderer();
	Camera camera = Camera();
	renderer.SetActiveCamera(&camera);

	/* Create the base 'game' */
	game = new GameName();
	game->renderer = &renderer;
	game->camera = &camera;
	game->input = new InputHandler();
	game->Load();

	while(!quit)
	{
		run();
	}

	game->Shutdown();
	delete game;

	Filesystem::Shutdown();

	return 0;
}