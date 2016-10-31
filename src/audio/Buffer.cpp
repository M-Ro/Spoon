#include <iostream>
#include "Buffer.h"

Buffer::Buffer(int buffer_id)
{
	a_buffer_id = buffer_id;
}

Buffer::~Buffer()
{
	alDeleteBuffers(1, &a_buffer_id);
}

void Buffer::BufferData(BufferChunk *chunk)
{
	alBufferData(a_buffer_id, chunk->fmt.format, chunk->data, chunk->length, chunk->fmt.frequency);

	ALenum error = alGetError();
	if(error != AL_NO_ERROR)
		std::cout << "Buffer::bufferData() alBufferData failed: AL Error code: " << error << std::endl;
}