#pragma once

#include <string>
#include <glm/glm.hpp>

#include "AudioObject.h"
#include "Source.h"

class AudioInstance
{
public:
	AudioInstance(std::string const& soundname, bool looping);
	~AudioInstance();

	void Update();

	void SetPosition(glm::vec3 pos);

	void SetVelocity(glm::vec3 vel);

	bool destroy;
protected:

	bool looping;

	AudioObject* audio_object;

	Source* source;
};