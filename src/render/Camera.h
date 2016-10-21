#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	/* Calculates Transform/View matrices from position/angle */
	void Update();

	void SetPosition(float x, float y, float z) { position = glm::vec3(x, y, z); }

	void SetAngle(float h, float v) { angle_h = h; angle_v = v; }

	glm::mat4 const &GetProjectionMatrix() { return projection_matrix; }

	glm::mat4 const &GetViewMatrix() { return view_matrix; }

private:
	glm::vec3 position;
	
	float angle_h;
	float angle_v;

	float fieldofview;

	glm::mat4 projection_matrix;
	glm::mat4 view_matrix;
};