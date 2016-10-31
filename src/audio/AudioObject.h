#pragma once

#include <map>

#include "StreamParser.h"
#include "Source.h"
#include "Buffer.h"

class AudioObject
{
public:

	AudioObject(const std::string &name);
	~AudioObject();

private:
	StreamParser *a_parser;

	char *data;

	bool valid;
};