#pragma once

#include "Buffer.h"

class Source
{
public:
	static void InitialiseSources();
	static void ShutdownSources();
	static Source* GetFreeSource();
	static void ReturnSourceToPool(Source* source) { a_sources.emplace(source->GetALSource(), source); }

	Source(ALuint source_id);
	~Source();

	void QueueBuffer(Buffer *buffer);

	ALuint GetALSource() { return this->a_source_id; }

private:

	static std::map<unsigned int, Source*> a_sources;

	static bool initialized;

	ALuint a_source_id;
};