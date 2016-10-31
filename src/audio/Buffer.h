#pragma once

#include "StreamParser.h"

class Buffer
{
public:
	Buffer(int buffer_id);
	~Buffer();

	void BufferData(BufferChunk *chunk);

private:

	ALuint a_buffer_id;
};