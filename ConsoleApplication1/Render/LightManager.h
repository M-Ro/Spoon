#pragma once

#include <vector>

#include <GL\glew.h>

#include "Light.h"

class LightManager
{
public:
	LightManager();
	~LightManager();

	void Update(GLuint program_id);

	Light* NewLight();

	void DeleteLight(Light* light);

	inline void SetDirty() { this->dirty = true; }

	inline GLuint GetUboHandle() { return ubo_handle; }

protected:
	std::vector<Light*> lights;

	bool dirty; // True if we need to recalculate UBO

	GLuint ubo_handle;
};