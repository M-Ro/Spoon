#include "../auxiliary/Filesystem.h"

#include "StreamParserWav.h"
#include "AudioObject.h"

AudioObject::AudioObject(const std::string &name)
{
	a_parser = nullptr;

	if(Filesystem::FileExists(name + ".wav"))
		a_parser = new StreamParserWav(name + ".wav");
	else
		return;

	if(!a_parser->Prepare())
		return;

	valid = false;
}

AudioObject::~AudioObject()
{
	if(a_parser != nullptr)
		a_parser->Destroy();

	delete a_parser;
}