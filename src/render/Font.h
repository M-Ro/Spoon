#pragma once

#include <string>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

enum FreetypeState
{
	Uninitialised,
	Initialised,
	Error
};

class Font
{
public:

	Font(std::string const &name, unsigned int size, glm::vec2 screenSize);
	~Font();

	void RenderText(std::string const &text, float x, float y);

private:

	FT_Face face; // The font

	GLuint vao; 	// VAO to contain VBO
	GLuint vbo;		// VBO reference

	Program program;

	GLint attribute_coord;
	GLint uniform_texture;
	GLint uniform_color;

	glm::vec2 screen;
};