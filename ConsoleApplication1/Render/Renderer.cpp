#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "../auxiliary/Config.h"
#include "../auxiliary/Filehandle.h"
#include "Renderer.h"
#include "Shader.h"
#include "OBJModel.h"

Renderer::Renderer()
{
	// Specify OpenGL 3.1
	std::cout << "Specifying OpenGL 3.3 Core" << std::endl;
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	/* Create SDL window */
	std::cout << "Creating SDL window" << std::endl;

	int width = Config::GetInt("r_width");
	int height = Config::GetInt("r_height");

	Uint32 flags = SDL_WINDOW_OPENGL;
	if(Config::GetInt("r_borderless") > 0)
	{
		flags |= SDL_WINDOW_BORDERLESS | SDL_WINDOW_MAXIMIZED;

		SDL_DisplayMode native;
		if(SDL_GetCurrentDisplayMode(0, &native) == 0)
		{
			width = native.w;
			height = native.h;
		}

	}
	else if(Config::GetInt("r_fullscreen") > 0)
		flags |= SDL_WINDOW_FULLSCREEN;

	window = SDL_CreateWindow("Spoon", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
	if (!window)
	{
		std::cout << "Error: Could not create SDL window: " << SDL_GetError() << std::endl;
		return;
	}
	
	/* Create SDL_GL context */
	std::cout << "Creating SDL_GL context" << std::endl;
	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	if (!gl_context)
	{
		std::cout << "Error: Could not create SDL_GL context: " << SDL_GetError() << std::endl;
		return;
	}

	InitialiseGlew();
	InitialiseOpenGL();

	SetVSync(Config::GetInt("r_vsync"));

	active_camera = 0;

	LoadTexture("textures/null"); // Default missing texture image

	font = new Font("emulogic", 16, glm::vec2(width, height));
}

Renderer::~Renderer()
{

}

bool Renderer::SetVSync(bool state)
{
	if (SDL_GL_SetSwapInterval(state ? 1 : 0) < 0)
	{
		std::cout << "Warning: Failed to enable VSync: " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}

bool Renderer::InitialiseGlew()
{
	std::cout << "Initialising GLEW" << std::endl;
	glewExperimental = GL_TRUE;
	GLenum glew_error = glewInit();
	if (glew_error != GLEW_OK)
	{
		std::cout << "Renderer::Renderer(): OpenGL Error " << glGetError() << std::endl;
		std::cout << "Error: Failed to initialise GLEW: " << glewGetErrorString(glew_error) << std::endl;
		return false;
	}
	
	return true;
}

bool Renderer::InitialiseOpenGL()
{
	/* Get screen surface from window */
	screen = SDL_GetWindowSurface(window);

	std::cout << "Initialising OpenGL" << std::endl;

	program_model = new Program();

	vshader = new Shader(Vertex);
	vshader->LoadShader("TransformVertexShader");
	program_model->AttachShader(vshader);

	fshader = new Shader(Fragment);
	fshader->LoadShader("TextureFragmentShader");
	program_model->AttachShader(fshader);
	
	program_model->Link();


	program_overlay = new Program();

	Shader *overlay_v = new Shader(Vertex);
	overlay_v->LoadShader("overlay");
	program_overlay->AttachShader(overlay_v);

	Shader *overlay_f = new Shader(Fragment);
	overlay_f->LoadShader("overlay");
	program_overlay->AttachShader(overlay_f);

	program_overlay->Link();

	glClearColor(0.22f, 0.22f, 0.22f, 1.0f); // Set clear color to gray // FIXME remove?

    glEnable(GL_DEPTH_TEST); // Enable Depth Testing
    glDepthFunc(GL_LESS);  // Set depth function -> closer priority
    glEnable(GL_CULL_FACE); // Enable face culling

	return true;
}

void Renderer::DrawModel(std::string const &modelname, glm::vec3 &position, glm::vec3 angles)
{
	/* Don't bother if we have no perspective to draw from */
	if(!active_camera)
		return;

	/* Check if the model is loaded, load if required */
	if(!models.count(modelname))
		LoadModel(modelname);

	Model *model = models.at(modelname);
	if(!model) // Invalid model, don't attempt to draw
		return;

	/* Iterate over each model section and draw the model */
	for(ModelSection *section : model->GetSections())
	{
		/* Check for null section - Unlikely, but try not to crash anyway */
		if(!section)
			continue;

		/* Get buffers/arrays for this model */
		GLuint vertex_array  = section->GetVertexArray();
		GLuint vertex_buffer = section->GetVertexBuffer();
		GLuint uv_buffer 	 = section->GetUVBuffer();
		GLuint normal_buffer  = section->GetNormalBuffer();

		if(!textures.count(section->GetMaterial())) // Load texture if required
			LoadTexture(section->GetMaterial());

		GLuint texture_id = (textures.at(section->GetMaterial()))->GetGLTexture(); // TODO handle this at loadtime
		GLuint matrix_id = glGetUniformLocation(program_model->GetProgram(), "MVP"); // model view projection handle
		GLuint ViewMatrixID = glGetUniformLocation(program_model->GetProgram(), "V");
		GLuint ModelMatrixID = glGetUniformLocation(program_model->GetProgram(), "M");
		GLuint sampler_tex_id  = glGetUniformLocation(program_model->GetProgram(), "myTextureSampler");
		GLuint LightID = glGetUniformLocation(program_model->GetProgram(), "lightPosition_worldspace");

		glUseProgram(program_model->GetProgram());

		glm::mat4 projection_matrix = active_camera->GetProjectionMatrix();
        glm::mat4 view_matrix = active_camera->GetViewMatrix();
        glm::mat4 model_matrix = glm::translate(glm::mat4(1.0), position);

        glm::mat4 rotation1, rotation2, rotation0;
        rotation0 = glm::eulerAngleX(angles[0]);
        rotation1 = glm::eulerAngleY(angles[1]);
        rotation2 = glm::eulerAngleZ(angles[2]);

        //glm::mat4 rotation = glm::eulerAngleXYZ(angles[0],angles[1],angles[2]);
        glm::mat4 rotation = rotation2*rotation1*rotation0;

        glm::mat4 modelviewprojection = projection_matrix * view_matrix * model_matrix * rotation;

        glBindVertexArray(vertex_array); // Bind VAO

        glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &modelviewprojection[0][0]); // Bind model view matrix to shader
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &model_matrix[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &view_matrix[0][0]);

		glUniform3f(LightID, 0, 50, 0); // position
        /* Bind material texture to unit 0 */
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id);

        // Set our "myTextureSampler" sampler to user Texture Unit 0
        glUniform1i(sampler_tex_id, 0);

        /* Load Vertex Buffer Object and UV Buffer Object */
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Draw mesh
        glDrawArrays(GL_TRIANGLES, 0, section->GetVertexCount());

        /* Unselect VBO's */
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);

        glBindVertexArray(0); // Unbind VAO
	}
}

void Renderer::DrawCModel(int type, glm::vec3 origin, glm::vec3 size, float r)
{
	/* Don't bother if we have no perspective to draw from */
	if(!active_camera)
		return;

	/* Check if the model is loaded, load if required */
	std::string modelname;
	if(type == 0)
		modelname = "cube";
	else
		modelname = "sphere";

	if(!models.count(modelname))
		LoadModel(modelname);

	Model *model = models.at(modelname);
	if(!model) // Invalid model, don't attempt to draw
		return;

	ModelSection *section = model->GetSections()[0];
	if(!section)
		return;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLuint vertex_array  = section->GetVertexArray();
	GLuint vertex_buffer = section->GetVertexBuffer();

	GLuint matrix_id = glGetUniformLocation(program_overlay->GetProgram(), "MVP"); // model view projection handle

	glUseProgram(program_overlay->GetProgram());

	glm::mat4 projection_matrix = active_camera->GetProjectionMatrix();
	glm::mat4 view_matrix = active_camera->GetViewMatrix();
	glm::mat4 model_matrix = glm::translate(glm::mat4(1.0), origin);
	glm::mat4 scale_matrix = type == 0 ? glm::scale(size) : glm::scale(glm::vec3(r*2, r*2, r*2));
	glm::mat4 modelviewprojection = projection_matrix * view_matrix * model_matrix * scale_matrix;

	glBindVertexArray(vertex_array); // Bind VAO
	glUniformMatrix4fv(matrix_id, 1, GL_FALSE, &modelviewprojection[0][0]); // Bind model view matrix to shader

	/* Load Vertex Buffer Object and UV Buffer Object */
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Draw mesh
	glDrawArrays(GL_TRIANGLES, 0, section->GetVertexCount());

	/* Unselect VBO's */
	glDisableVertexAttribArray(0);
	glBindVertexArray(0); // Unbind VAO

	glDisable(GL_BLEND);
}

void Renderer::Flip()
{
	SDL_GL_SwapWindow(window); // Update screen

	GLuint error = glGetError();
	if(error != 0)
		std::cout << "OpenGL Error: " << error << std::endl;
}

void Renderer::LoadTexture(std::string const &filename)
{
	Texture *texture = new Texture(filename);

	std::string filepath = filename + ".dds";
	filepath = filename + ".png";
	Filehandle file = Filehandle(filepath, true);
	if(!file.IsOpen())
	{
		std::cout << "Warning: Couldn't find image: " << filepath << std::endl;
		delete texture;

		textures.emplace(filename, textures.at("textures/null"));
		return;
	}

	std::istream *stream = file.GetIStream();
	texture->LoadPNG(*stream);

	textures.insert(std::pair<std::string, Texture *>(filename, texture));
}

void Renderer::LoadModel(std::string const &filename)
{
	std::string filepath = filename;

	Model *model = 0;

	filepath = "models/" + filename + ".obj";
	model = OBJModel::Load(filepath);

	if(model)
		model->Upload();

	models.insert(std::pair<std::string, Model *>(filename, model));
}

void Renderer::PushModel(std::string name, Model* model)
{
	if (this->models.count(name)) {
		std::cout << "Warning: PushModel overriding " << name << std::endl;
	}

	this->models.emplace(name, model);
}


void Renderer::PrecacheModel(std::string const &modelname)
{
	/* Load the model */
	if(!models.count(modelname))
		LoadModel(modelname);

	Model *model = models.at(modelname);
	if(!model) // Invalid model, don't attempt to draw
	{
		std::cout << "Precache: Failed to precache model: " << modelname << std::endl;
		return;
	}

	/* Precache material/textures used by model */
	for(ModelSection *section : model->GetSections())
		if(!textures.count(section->GetMaterial())) // Load texture if required
			LoadTexture(section->GetMaterial());
}

Texture* Renderer::GetTexture(std::string const& texname)
{
	/* Check if the model is loaded, load if required */
	if (!textures.count(texname))
		LoadTexture(texname);

	return textures.at(texname);
}