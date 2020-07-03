#pragma once

#include <AL/al.h>

#include "../auxiliary/Filehandle.h"

struct BufferFormat
{
	ALenum format;
	ALuint frequency;
};

struct BufferChunk
{
	BufferFormat fmt;
	char *data;
	unsigned long length;
};

const static int CHUNKSIZE = 1024 * 1024 * 16; // 16MB Chunk size to read (bytes), -1 = all data

class StreamParser
{
public:

	StreamParser* GetFileParser(std::string const& name);

	enum State { Valid, Error, Finalized };

	StreamParser(const std::string &filepath);
	virtual ~StreamParser() {};

	virtual void Destroy() = 0;

	virtual int Prepare() = 0;

	virtual BufferChunk* ReadChunk() = 0;

protected:

	State state;

	std::string a_filepath;
	Filehandle *a_file;

	BufferFormat a_file_format;
};