#include <iostream>
#include "AudioSystem.h"

const int MAX_BUFFERS = 256;
const int MAX_SOURCES = 32;

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

	// preallocate AL buffers
	ALuint al_buffers[MAX_BUFFERS];
	alGenBuffers(MAX_BUFFERS, al_buffers);

	ALenum error;
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		std::cout << "AudioSystem::AudioSystem(): alGenBuffers() failed: " << error << std::endl;
		return;
	}

	for(int i=0; i<MAX_BUFFERS; i++)
	{
		Buffer *buffer = new Buffer(al_buffers[i]);
		a_buffers[i] = buffer;
	}

	std::cout << "AudioSystem: Allocated " << a_buffers.size() << " audio buffers" << std::endl;

	// preallocate AL sources
	ALuint al_sources[MAX_SOURCES];
	alGenSources(MAX_SOURCES, al_sources);

	if ((error = alGetError()) != AL_NO_ERROR)
	{
		std::cout << "AudioSystem::AudioSystem(): alGenSources(): " << error << std::endl;
		return;
	}

	for(int i=0; i<MAX_SOURCES; i++)
	{
		Source *source = new Source(al_sources[i]);
		a_sources[i] = source;
	}

	std::cout << "AudioSystem: Allocated " << a_sources.size() << " audio sources" << std::endl;
}

AudioSystem::~AudioSystem()
{
	/* Clear each AL Source */
	for(auto &kv : a_sources)
	{
		Source *source = kv.second;
		if(source != 0)
			delete source;
	}

	a_sources.clear();

	/* Clear each AL Buffer */
	for(auto &kv : a_buffers)
	{
		Buffer *buffer = kv.second;
		if(buffer != 0)
			delete buffer;
	}

	a_buffers.clear();

	/* Delete each Audio Object */
	for(auto &kv : a_sounds)
	{
		AudioObject *sound = kv.second;
		if(sound != 0)
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

int AudioSystem::Play(const std::string &soundname, bool loop)
{
	return -1; // fixme stub
}

void AudioSystem::Stop(int id)
{
	return; // fixme stub
}

Buffer* AudioSystem::GetFreeBuffer()
{
	return nullptr; // fixme stub
}

Source* AudioSystem::GetFreeSource()
{
	return nullptr; // fixme stub
}