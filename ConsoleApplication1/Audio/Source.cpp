#include <iostream>

#include "Source.h"

std::map<unsigned int, Source*> Source::a_sources;
bool Source::initialized = false;
const int MAX_SOURCES = 128;

void Source::InitialiseSources()
{
	// preallocate AL sources
	ALuint al_sources[MAX_SOURCES];
	alGenSources(MAX_SOURCES, al_sources);

	ALenum error;
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		std::cout << "AudioSystem::AudioSystem(): alGenSources(): " << error << std::endl;
		return;
	}

	for (int i = 0; i < MAX_SOURCES; i++)
	{
		Source* source = new Source(al_sources[i]);
		a_sources[i] = source;
	}

	initialized = true;

	std::cout << "AudioSystem: Allocated " << a_sources.size() << " audio sources" << std::endl;
}

void Source::ShutdownSources()
{
	/* Clear each AL Source */
	for (auto& kv : a_sources)
	{
		Source* source = kv.second;
		if (source != 0)
			delete source;
	}

	a_sources.clear();
}

Source* Source::GetFreeSource()
{
	if (!initialized) {
		InitialiseSources();
	}

	if (!a_sources.size()) {
		std::cout << "Warning: No free AL sources" << std::endl;
		return nullptr;
	}

	Source* source = a_sources.begin()->second;
	a_sources.erase(a_sources.begin());

	return source;
}

Source::Source(ALuint source_id)
{
	a_source_id = source_id;
}

Source::~Source()
{
	alDeleteSources(1, &a_source_id);
}

void Source::QueueBuffer(Buffer *buffer)
{
	alSourcei(this->a_source_id, AL_BUFFER, buffer->GetALBuffer());
}