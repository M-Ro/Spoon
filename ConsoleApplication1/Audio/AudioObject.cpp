#include <iostream>

#include "../auxiliary/Filesystem.h"

#include "StreamParserWav.h"
#include "AudioObject.h"

std::map<std::string, AudioObject*> AudioObject::a_sounds;

AudioObject::AudioObject(std::string const &name)
{
	valid = false;
	finalized = false;
	a_parser = nullptr;
	data = nullptr;

	if(Filesystem::FileExists("sounds/" + name + ".wav"))
		a_parser = new StreamParserWav("sounds/" + name + ".wav");
	else {
		std::cout << "Could not find readable file for " << name << std::endl;
		return;
	}

	if(!a_parser->Prepare())
		return;

	valid = true;

	this->chunks.push_back(a_parser->ReadChunk());

	Buffer* buffer = Buffer::GetFreeBuffer();
	buffer->BufferData(this->chunks[0]);

	this->buffers.push_back(buffer);
}

AudioObject::~AudioObject()
{
	if(a_parser != nullptr)
		a_parser->Destroy();

	delete a_parser;
}

AudioObject* AudioObject::GetAudioObject(std::string const& soundname)
{
	if (a_sounds.count(soundname)) {
		return a_sounds.at(soundname);
	}

	AudioObject* object = new AudioObject(soundname);
	a_sounds.emplace(soundname, object);

	return object;
}