#pragma once

#include "StreamParser.h"

class StreamParserWav : public StreamParser
{
public:

	StreamParserWav(const std::string &path) : StreamParser(path) {}

	virtual void Destroy();

	virtual int Prepare();

	virtual BufferChunk* ReadChunk();

private:
	unsigned int dataLen;
};