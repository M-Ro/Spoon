#pragma once

#include <string>

#ifdef _WIN32
	#include <windows.h> // Needed before including GL.h on WIN32
#endif

#include <GL\glew.h>

class Texture
{
public:

	Texture(std::string const &filename);
	~Texture();

	bool LoadPNG(std::istream &stream);

	/* Getters */

	int GetWidth() { return width; }
	int GetHeight() { return height; }
	int GetBPP() { return bpp; }

	GLuint GetGLTexture() { return gl_texture; }

private:
	std::string name; // resource name, used for error detection

	unsigned int width;
	unsigned int height;
	unsigned int channels;
	unsigned int bpp;

	GLuint gl_texture;
};