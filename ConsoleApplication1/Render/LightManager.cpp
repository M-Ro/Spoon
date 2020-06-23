#include <iostream>

#include "LightManager.h"

#include <glm/vec4.hpp>

const size_t MAX_LIGHTS = 256;

#pragma pack(push, 1)
typedef struct {
	glm::vec4 posAtten[MAX_LIGHTS];
	glm::vec4 rgbPower[MAX_LIGHTS];
} lightbuffer_t;
#pragma pack(pop)

LightManager::LightManager()
{
	this->dirty = true;
	this->ubo_handle = 0;
}

LightManager::~LightManager()
{

}

void LightManager::Update(GLuint program_id)
{
	// TODO here we check all lights from camera matrix and purge any out of range

	if (!this->dirty) {
		return;
	}

	lightbuffer_t buffer;
	memset(&buffer, 0, sizeof(lightbuffer_t));

	size_t i = 0;
	while (i < this->lights.size()) {
		Light* l = this->lights[i];

		glm::vec3 pos = l->GetPosition();
		glm::vec3 rgb = l->GetColor();
		float atten = l->GetAttenuation();
		float power = l->GetPower();

		buffer.posAtten[i] = glm::vec4(pos.x, pos.y, pos.z, atten);
		buffer.rgbPower[i] = glm::vec4(rgb.r, rgb.g, rgb.b, power);

		i++;
	}

	glUseProgram(program_id);

	if (this->ubo_handle) {
		glDeleteBuffers(1, &ubo_handle);
		this->ubo_handle = 0;
	}

	GLuint nLightUniformIndex = glGetUniformLocation(program_id, "num_lights");
	glUniform1f(nLightUniformIndex, (float)this->lights.size());

	GLuint lightUniformIndex = glGetUniformBlockIndex(program_id, "Lights");
	glGenBuffers(1, &ubo_handle);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(lightbuffer_t), &buffer,
		GL_DYNAMIC_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, lightUniformIndex, ubo_handle);

	//giggety
	this->dirty = false;
}

Light* LightManager::NewLight()
{
	Light* l = new Light();

	this->lights.push_back(l);
	this->dirty = true;

	return l;
}

void LightManager::DeleteLight(Light* light)
{
	auto it = this->lights.begin();
	while (it != this->lights.end()) {
		if (*it == light) {
			lights.erase(it);
			delete light;
			return;
		}
		++it;
	}
}
