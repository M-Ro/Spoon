#include <iostream>
#include <cstring>

#include "StreamParserWav.h"

int StreamParserWav::Prepare()
{
	a_file = new Filehandle(a_filepath);

	if(!a_file->IsOpen())
	{
		Destroy();
		return 0;
	}

	// Process the wav header
	unsigned char header[44];
	if(a_file->ReadBytes(header, 44) < 44)
	{
		std::cout << "Error: StreamParserWav::Prepare(): " << a_filepath << ": Could not read header" << std::endl;
		Destroy();
		return 0;
	}

	if(strncmp((char*)header, "RIFF", 4) != 0 ) // Chunk ID - Should be "RIFF"
	{
		std::cout << "Error: StreamParserWav::Prepare(): " << a_filepath << ": Header does not match RIFF specification" << std::endl;
		Destroy();
		return 0;
	}

	// Between RIFF and WAVE - 4 bytes specifying header chunk size (ignored)

	if(strncmp((char*)header+8, "WAVE", 4) != 0 )
	{
		std::cout << "Error: StreamParserWav::Prepare(): " << a_filepath << ": Not a RIFF WAVE file." << std::endl;
		Destroy();
		return 0;
	}

	if(strncmp((char*)header+12, "fmt ", 4) != 0 )
	{
		std::cout << "Error: StreamParserWav::Prepare(): " << a_filepath << ": Corrupt format chunk." << std::endl;
		Destroy();
		return 0;
	}

	// 4 bytes here specifying header sub chunk size (ignored)

	unsigned short audioFormat = header[20] + (header[21] << 8);
	if(audioFormat != 1) // Audio format must be 1 - Uncompressed PCM
	{
		std::cout << "Error: StreamParserWav::Prepare(): " << a_filepath << ": File is not PCM. Possibly compressed." << std::endl;
		Destroy();
		return 0;
	}

	unsigned short numchannels = header[22] + (header[23] << 8);
	unsigned int samplerate = header[24] + (header[25] << 8) + (header[26] << 16) + (header[27] << 24);
	// 4 bytes specifying byterate (samplerate * nchannels * bitspersample/8) ignored
	// 2 bytes specifying blockalign (ignored)
	unsigned int bitspersample = header[34] + (header[35] << 8);

	unsigned int dataSize = 0; // Size of the raw PCM data

	if(strncmp((char *)header+36, "LIST",4) == 0) // Meta Length. Skip over metadata
	{
		unsigned int metalength = header[40] + (header[41] << 8) + (header[42] << 16) + (header[43] << 24);

		a_file->Seek(a_file->Tell() + metalength);
		a_file->ReadBytes(header+40, 4);

		dataSize = header[40] + (header[41] << 8) + (header[42] << 16) + (header[43] << 24);
	}
	else if(strncmp((char *)header+36, "data", 4) == 0) // subchunk 2 id 'data'
	{
		dataSize = header[40] + (header[41] << 8) + (header[42] << 16) + (header[43] << 24);
	}
	else
	{
		std::cout << "Error: StreamParserWav::Prepare(): " << a_filepath << ": Corrupt DATA chunk!" << std::endl;
		Destroy();
		return 0;
	}

	if(dataSize > 0)
		dataLen = dataSize;
	else
	{
		std::cout << "Error: StreamParserWav::Prepare(): " << a_filepath << ": Zero data length!" << std::endl;
		Destroy();
		return 0;
	}

	a_file_format.frequency = samplerate;
	if (bitspersample == 16)
	{
		if (numchannels == 1)
			a_file_format.format = AL_FORMAT_MONO16;
		else if(numchannels == 2)
			a_file_format.format = AL_FORMAT_STEREO16;
	} else if (bitspersample == 8)
	{
		if (numchannels == 1)
			a_file_format.format = AL_FORMAT_MONO8;
		else if (numchannels == 2)
			a_file_format.format = AL_FORMAT_STEREO8;
	}

	state = State::Valid;
	return 1;
}

BufferChunk* StreamParserWav::ReadChunk()
{
	if(state == State::Error)
		return nullptr;

	long bytes;
	if(CHUNKSIZE > 0)
		bytes = CHUNKSIZE;
	else
		bytes = dataLen;

	char *data = new char[bytes];

	long bytesRead = 0;
	while(bytesRead < bytes)
	{
		long dataRead = a_file->ReadBytes(data+bytesRead, FILE_MAXREAD);
		if(dataRead == -1)
		{
			std::cout << "Error: StreamParserWav::ReadChunk(): " << a_filepath << ": Failed to reade data!" << std::endl;
			Destroy();
			return nullptr;
		}
	}

	BufferChunk *chunk = new BufferChunk();
	chunk->data = data;
	chunk->length = bytesRead;
	chunk->fmt = a_file_format;

	return chunk;
}

void StreamParserWav::Destroy()
{
	if(a_file != 0)
	{
		if(a_file->IsOpen())
			a_file->Close();

		delete a_file;
	}

	state = State::Error;
}