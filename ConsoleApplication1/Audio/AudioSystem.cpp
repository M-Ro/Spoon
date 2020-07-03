#include <iostream>
#include "AudioSystem.h"

#include "Source.h"
#include "Buffer.h"

AudioSystem::AudioSystem()
{
	a_context = 0;
	a_device = alcOpenDevice(0);
	if(!a_device)
	{
		std::cout << "AudioSystem::AudioSystem(): alcOpenDevice() failed" << std::endl;
		return;
	}

	a_context = alcCreateContext(a_device, 0);
	if(!a_context)
	{
		std::cout << "AudioSystem::AudioSystem(): alcCreateContext() failed" << std::endl;
		return;
	}

	alcMakeContextCurrent(a_context);
}

AudioSystem::~AudioSystem()
{
	Source::ShutdownSources();
	Buffer::ShutdownBuffers();

	/* Delete each Audio Object */
	for (auto& kv : a_sounds)
	{
		AudioObject* sound = kv.second;
		if (sound != 0)
			delete sound;
	}

	a_sounds.clear();

	/* Release the context and close the AL device */
	alcMakeContextCurrent(0);
    alcDestroyContext(a_context);
    alcCloseDevice(a_device);
}

void AudioSystem::Update(glm::vec3 pos, glm::vec3 forward, glm::vec3 up, glm::vec3 vel)
{
	/* Update the listener */
	alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
	alListener3f(AL_VELOCITY, vel.x, vel.y, vel.z);

	ALfloat orientation[6] = { forward.x, forward.y, forward.z, up.x, up.y, up.z };
	alListenerfv(AL_ORIENTATION, orientation); 	// Orientation ...

	/* Todo: Handle stream updates */
}

void AudioSystem::Stop(AudioObject *object)
{
	return; // fixme stub
}