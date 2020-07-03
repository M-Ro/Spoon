#include <iostream>

#include "Filehandle.h"

Filehandle::Filehandle(const std::string &path, bool readonly)
{
	file = nullptr;
	cbuf = nullptr;
	sbuf = nullptr;
	stream = nullptr;

	Open(path, readonly);
}

Filehandle::~Filehandle()
{
	if (this->file) {
		PHYSFS_close(file);
	}

	if (this->stream) {
		delete this->stream;
	}

	if (this->sbuf) {
		delete this->sbuf;
	}

	if (this->cbuf) {
		delete this->cbuf;
	}
}

int Filehandle::Open(const std::string &path, bool readonly)
{
	if(file != nullptr)
		Close();

	if(!readonly)
		file = PHYSFS_openWrite(path.c_str());
	else
		file = PHYSFS_openRead(path.c_str());

	if (file == nullptr) {
		std::cout << "Failed to open " << path << ": " << PHYSFS_getLastError() << std::endl;
		return 0;
	}

	return 1;
}

int Filehandle::Close()
{
	int ret = PHYSFS_close(file);
	this->file = nullptr;

	return ret;
}

sint64 Filehandle::ReadBytes(void *buffer, uint32 count)
{
	if(count >= 1024)
		std::cout << "Warning: Filehandle::readBytes: count possibly too large " << count << std::endl;

	return PHYSFS_readBytes(file, buffer, count);
}

unsigned char *Filehandle::ReadFile()
{
	sint64 file_size = Size();
	unsigned char* file_buf = new unsigned char[file_size + 1];

	sint64 total_read = 0;
	while(total_read < file_size)
	{
		uint32 readsize = 512;
		if (file_size - total_read < readsize) {
			readsize = file_size - total_read;
		}

		sint64 bytes_read = ReadBytes(file_buf+total_read, readsize);
		if(bytes_read == -1)
		{
			std::cout << "Failed read: " << PHYSFS_getLastError() << std::endl;
			return 0;
		}

		total_read += bytes_read;
	}

	file_buf[file_size] = '\0';
	return file_buf;
}

std::istream *Filehandle::GetIStream()
{
	if (this->stream == nullptr) {
		this->cbuf = ReadFile();
		this->sbuf = new membuf((char*)cbuf, (char*)cbuf + Size() - 1);
		Close();
		this->stream = new std::istream(sbuf);
	}
	
	return this->stream;
}

sint64 Filehandle::Tell()
{
	return PHYSFS_tell(file);
}

int Filehandle::Seek(uint64 pos)
{
	return PHYSFS_seek(file, pos);
}

int Filehandle::Eof()
{
	return PHYSFS_eof(file);
}

sint64 Filehandle::Size()
{
	return PHYSFS_fileLength(file);
}

bool Filehandle::IsOpen()
{
	if(file == nullptr)
		return false;
	if(Tell() == -1)
		return false;

	return true;
}