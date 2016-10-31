#pragma once

#include <string>
#include <list>
#include <map>

#include <AL/al.h>
#include <AL/alc.h>

#include <glm/glm.hpp>

#include "AudioObject.h"
#include "Buffer.h"
#include "Source.h"

class AudioSystem
{
public:

	AudioSystem();
	~AudioSystem();

	int Play(const std::string &soundname, bool loop=false);

	void Stop(int id);

	void Update(glm::vec3 pos, glm::vec3 forward, glm::vec3 up, glm::vec3 vel);

	Buffer* GetFreeBuffer();

	Source* GetFreeSource();

private:

	ALCdevice *a_device;
	ALCcontext *a_context;

	std::map<unsigned int, Buffer *> a_buffers;
	std::map<unsigned int, Source *> a_sources;

	std::map<std::string, AudioObject *> a_sounds;

	//std::map<int, Source *> a_sources;
};