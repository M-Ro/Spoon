#include <iostream>
#include "Buffer.h"

std::map<unsigned int, Buffer*> Buffer::a_buffers;
const int MAX_BUFFERS = 512;
bool Buffer::initialized = false;

void Buffer::InitialiseBuffers()
{
	// preallocate AL buffers
	ALuint al_buffers[MAX_BUFFERS];
	alGenBuffers(MAX_BUFFERS, al_buffers);

	ALenum error;
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		std::cout << "AudioSystem::AudioSystem(): alGenBuffers() failed: " << error << std::endl;
		return;
	}

	for (int i = 0; i < MAX_BUFFERS; i++)
	{
		Buffer* buffer = new Buffer(al_buffers[i]);
		a_buffers[i] = buffer;
	}

	initialized = true;

	std::cout << "AL Buffers: Allocated " << a_buffers.size() << " audio buffers" << std::endl;

}

void Buffer::ShutdownBuffers()
{
	/* Clear each AL Buffer */
	for (auto& kv : a_buffers)
	{
		Buffer* buffer = kv.second;
		if (buffer != 0)
			delete buffer;
	}

	a_buffers.clear();
}

Buffer* Buffer::GetFreeBuffer()
{
	if (!initialized) {
		InitialiseBuffers();
	}

	if (!a_buffers.size()) {
		std::cout << "Warning: No free AL buffers" << std::endl;
		return nullptr;
	}

	Buffer* buffer = a_buffers.begin()->second;
	a_buffers.erase(a_buffers.begin());

	return buffer;
}

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