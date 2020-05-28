#include "Source.h"

Source::Source(ALuint source_id)
{
	a_source_id = source_id;
}

Source::~Source()
{
	alDeleteSources(1, &a_source_id);
}

void Source::QueueBuffer(Buffer *buffer)
{

}