#pragma once

#include <map>

#include "StreamParser.h"

class Buffer
{
public:

	static void InitialiseBuffers();
	static void ShutdownBuffers();
	static Buffer* GetFreeBuffer();

	Buffer(int buffer_id);
	~Buffer();

	void BufferData(BufferChunk *chunk);

	ALuint GetALBuffer() { return this->a_buffer_id; }

private:

	static std::map<unsigned int, Buffer*> a_buffers;

	static bool initialized;

	ALuint a_buffer_id;
};