#include <iostream>

#include "Program.h"

Program::Program()
{
	program_id = glCreateProgram(); // Create program
}

Program::~Program()
{
	glDeleteProgram(program_id);
}

void Program::AttachShader(Shader *shader)
{
	glAttachShader(program_id, shader->GetShader()); // Attach shader to GL program
}

bool Program::Link()
{
	glLinkProgram(program_id); // Link program

	/* Check for errors */
	GLint program_success = 0;
	glGetProgramiv(program_id, GL_LINK_STATUS, &program_success);
	if (program_success != GL_TRUE)
	{
		std::cout << "Error linking program: " << program_id << std::endl;
		PrintLog();
		return false;
	}

	return true;
}

void Program::PrintLog()
{
	if (!glIsProgram(program_id)) // Not a program
	{
		std::cout << "Warning: OpenGL Program " << program_id << " is not a program" << std::endl;
		return;
	}

	int max_length = 0;
	glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &max_length); //Get info string length

	/* Get program log */
	char *info_log = new char[max_length];
	int info_log_length = 0;
	glGetProgramInfoLog(program_id, max_length, &info_log_length, info_log);
	if (info_log_length > 0)
		std::cout << "OpenGL Program Log:" << std::endl << info_log << std::endl;

	delete[] info_log;
}
