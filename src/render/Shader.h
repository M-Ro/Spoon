#pragma once

#include <string>

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