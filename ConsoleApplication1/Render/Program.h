#pragma once

#include "Shader.h"

class Program
{
public:
	Program();
	~Program();

	void AttachShader(Shader *shader);
	bool Link();

	void PrintLog();

	/* Getters */

	GLuint GetProgram() { return program_id; }

	GLint GetUniform(const char *name) { return glGetUniformLocation(program_id, name); }

	GLint GetAttribute(const char *name) { return glGetAttribLocation(program_id, name); }

private:
	GLuint program_id;

};