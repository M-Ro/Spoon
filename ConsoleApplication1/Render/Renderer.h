#pragma once

#include <map>

#ifdef _WIN32
#include <windows.h> // Needed before including GL.h on WIN32
#endif
#include <SDL2/SDL.h>
#include <GL\glew.h>
#include <SDL2/SDL_opengl.h>
#include <GL\glu.h>

#include "Program.h"
#include "Texture.h"
#include "Model.h"
#include "Camera.h"
#include "Font.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	bool SetVSync(bool state);
	
	void DrawModel(std::string const &modelname, glm::vec3 &position, glm::vec3 angles);

	void DrawCModel(int type, glm::vec3 origin, glm::vec3 size, float r);

	void DrawText(std::string const &text, float x, float y) { if(font) font->RenderText(text, x, y); }

	void SetActiveCamera(Camera *camera) { this->active_camera = camera; }

	void PushModel(std::string name, Model* model);

	void Flip();

	void PrecacheModel(std::string const &filename);

	Texture* GetTexture(std::string const& texname);

private:
	bool InitialiseGlew();
	bool InitialiseOpenGL();

	void PrintProgramLog(GLuint program);

	void LoadTexture(std::string const &filename);

	void LoadModel(std::string const &modelname);

	SDL_Window *window;
	SDL_Surface *screen;

	Font *font;

	Program *program_model;
	Program *program_overlay; // Renders object as a transparent overlay. Useful for bboxes etc
	Shader *vshader;	// FIXME remove?
	Shader *fshader;	// ''

	Camera *active_camera;

	std::map<std::string, Texture *> textures;
	std::map<std::string, Model *> models;
};
