#pragma once

#include <map>

#include <SDL2/SDL.h>
#include <GL\glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL\glu.h>

#include "Program.h"
#include "Texture.h"
#include "Model.h"
#include "Camera.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	bool SetVSync(bool state);
	
	void DrawModel(std::string const &modelname, glm::vec3 &position);

	void SetActiveCamera(Camera *camera) { this->active_camera = camera; }

	void Flip();

private:
	bool InitialiseGlew();
	bool InitialiseOpenGL();

	void PrintProgramLog(GLuint program);

	void LoadTexture(std::string const &filename);

	void LoadModel(std::string const &filename);

	SDL_Window *window;
	SDL_Surface *screen;

	Program *program_model;
	Shader *vshader;
	Shader *fshader;

	Camera *active_camera;

	std::map<std::string, Texture *> textures;
	std::map<std::string, Model *> models;
};
