#include "Light.h"

// Just spawn a light with defaults
Light::Light() : Light(glm::vec3(0, 0, 0), glm::vec3(1.0f, 1.0f, 1.0f), 150.0f, 2.0f)
{

}

Light::Light(glm::vec3 pos, glm::vec3 col, float power, float atten)
{
	this->inView = true;

	this->rgb = col;
	this->pos = pos;
	this->power = power;
	this->atten = atten;
}
