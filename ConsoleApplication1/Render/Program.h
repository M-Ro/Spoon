#pragma once

#include <vector>

#include "Shader.h"

class Program
{
public:
	Program();
	~Program();

	void AttachShader(ShaderType const& shader_type, std::string const& shader_name);
	bool Link();

	void PrintLog();

	/* Getters */

	GLuint GetProgram() { return program_id; }

	GLint GetUniform(const char *name) { return glGetUniformLocation(program_id, name); }

	GLint GetAttribute(const char *name) { return glGetAttribLocation(program_id, name); }

private:
	GLuint program_id;

	std::vector<Shader*> shaders;

};