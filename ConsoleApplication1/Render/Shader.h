#pragma once

#include <string>

#ifdef _WIN32
#include <windows.h> // Needed before including GL.h on WIN32
#endif

#include <GL\glew.h>

enum ShaderType
{
	Vertex,
	Fragment
};

class Shader
{
public:
	Shader(ShaderType type);
	~Shader();

	void LoadShader(std::string const &name);

	void PrintLog();

	/* Getters */

	GLuint GetShader() { return shader; }

private:
	ShaderType type;

	GLuint shader;

};