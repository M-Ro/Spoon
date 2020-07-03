#pragma once

#include <vector>

#include "StreamParser.h"
#include "Buffer.h"

class AudioObject
{
public:

	AudioObject(const std::string &name);
	~AudioObject();

	static AudioObject *GetAudioObject(std::string const& soundname);

	std::vector<Buffer*> const& GetBuffers() { return buffers; }

private:

	StreamParser *a_parser;

	std::vector<BufferChunk*> chunks; // Chunks associated with each buffer
	std::vector<Buffer *> buffers; // Buffers in use by this audio object

	static std::map<std::string, AudioObject*> a_sounds;

	char *data;

	bool finalized;
	bool valid;
};