#include <iostream>

#include "Shader.h"
#include "Program.h"
#include "Font.h"
#include "../auxiliary/Filehandle.h"

#include <windows.h> // Needed before including GL.h on WIN32
#include <GL\glu.h>

static FT_Library ft; // Freetype library reference
static FreetypeState state;

Font::Font(std::string const &name, unsigned int size, glm::vec2 screenSize)
{
	screen = screenSize;

	/* Initialise Freetype library */
	if(state == Uninitialised)
	{
		if(FT_Init_FreeType(&ft))
		{
			state = Error;
			std::cout << "Error: Failed to initialise Freetype" << std::endl;
			return;
		}
		else
		{
			state = Initialised;
			std::cout << "Freetype initialised" << std::endl;
		}
	}

	std::string font_path = "fonts/" + name + ".ttf";

	Filehandle filehandle = Filehandle(font_path, true);
	if(!filehandle.IsOpen())
	{
		std::cout << "Warning: Failed to open font " << font_path << std::endl;
		return;
	}

	sint64 file_size = filehandle.Size();
	FT_Byte *file_buf = filehandle.ReadFile(); // FIXME delete this
	filehandle.Close();

	/* Attempt to load font */
	if(FT_New_Memory_Face(ft, file_buf, file_size, 0, &face))
	{
		std::cout << "Warning: TTF Failed to parse font " << name << std::endl;
		return;
	}

	std::cout << "Loaded: " << font_path << std::endl;

	FT_Set_Pixel_Sizes(face, 0, size); // Set font size

	/* Load the shader program used for rendering */
	Shader *vshader = new Shader(Vertex);
	vshader->LoadShader("Glyph");
	program.AttachShader(vshader);

	Shader *fshader = new Shader(Fragment);
	fshader->LoadShader("Glyph");
	program.AttachShader(fshader);

	program.Link();

	attribute_coord = program.GetAttribute("coord");
	uniform_texture = program.GetUniform("tex");
	uniform_color = program.GetUniform("textColor");

	/* Generate VAO and VBO */
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo); // Generate VBO
}

Font::~Font()
{
	// Free VBO
}

void Font::RenderText(std::string const &text, float x, float y)
{
	if(text.empty())
		return;

	/* Bind VAO */
	glBindVertexArray(vao);

	/* Enable alpha blending, disable face culling */
	glEnable(GL_BLEND); // Enable alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);

	glUseProgram(program.GetProgram());

	/* Generate and setup the GL texture we will use to render glyphs */
	GLuint glyph_tex;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &glyph_tex);
	glBindTexture(GL_TEXTURE_2D, glyph_tex);
	glUniform1i(uniform_texture, 0); // ERR

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable 4-byte alignment

	/* Clamp edges and enable linear interpolation */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/* Bind VAO and VBO */
    glEnableVertexAttribArray(attribute_coord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);

    GLfloat fontcolor[4] = { 0.95f, 0.95f, 0.95f, 1.0f };
	glUniform4fv(uniform_color, 1, fontcolor);

	for(const char &c : text)
	{
		if(FT_Load_Char(face, c, FT_LOAD_RENDER)) // Load the glyph
        	continue;

        FT_GlyphSlot glyph = face->glyph;

        /* Upload greyscale glyph to gpu */
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, glyph->bitmap.width, glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, glyph->bitmap.buffer);
        /* Calculate the vertex and texture coordinates */
        float x2 = x + glyph->bitmap_left * (2.0f/screen.x);
        float y2 = -y - glyph->bitmap_top * (2.0f/screen.y);
        float w = glyph->bitmap.width * (2.0f /screen.x);
        float h = glyph->bitmap.rows * (2.0f/screen.y);
 
        GLfloat box[4][4] = 
        {
        	{x2, -y2, 0, 0},
            {x2 + w, -y2, 1, 0},
            {x2, -y2 - h, 0, 1},
            {x2 + w, -y2 - h, 1, 1},
        };

        /* Draw the character on the screen */
        glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
 
        /* Advance the cursor to the start of the next character */
        x += (glyph->advance.x >> 6) * (2.0f/screen.x); // FIXME Why shift 6?
        y += (glyph->advance.y >> 6) * (2.0f/screen.y);
	}

	glDisableVertexAttribArray(attribute_coord);
	glBindVertexArray(0); // Cleanup, unbind VAO
    glDeleteTextures(1, &glyph_tex);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE); // Re-enable face culling
}