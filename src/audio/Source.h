#pragma once

#include "Buffer.h"

class Source
{
public:
	Source(ALuint source_id);
	~Source();

	void QueueBuffer(Buffer *buffer);

private:

	ALuint a_source_id;
};