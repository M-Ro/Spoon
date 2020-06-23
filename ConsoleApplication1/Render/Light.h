#pragma once

#include <glm/vec3.hpp>

// Renderer's representation of a point light
class Light
{
public:
	Light(glm::vec3 pos, glm::vec3 col, float power, float atten);
	Light();

	void SetColor(glm::vec3 col) { this->rgb = col; }

	void SetAttenuation(float atten) { this->atten = atten; }

	inline void SetPosition(glm::vec3 pos) { this->pos = pos; }

	inline void SetPower(float power) { this->power = power; }

	glm::vec3 const& GetPosition() { return pos; }

	glm::vec3 const& GetColor() { return rgb; }

	float const& GetPower() { return power; }

	float const& GetAttenuation() { return atten; }

protected:

	glm::vec3 pos;
	glm::vec3 rgb;
	float atten;
	float power;

	bool inView;
};