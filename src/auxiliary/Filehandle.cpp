#include <iostream>

#include "Filehandle.h"

Filehandle::Filehandle(const std::string &path, bool readonly)
{
	file = nullptr;
	Open(path, readonly);
}

Filehandle::~Filehandle()
{

}

int Filehandle::Open(const std::string &path, bool readonly)
{
	if(file != nullptr)
		Close();

	if(!readonly)
		file = PHYSFS_openWrite(path.c_str());
	else
		file = PHYSFS_openRead(path.c_str());

	if(file == nullptr)
		return 0;

	return 1;
}

int Filehandle::Close()
{
	return PHYSFS_close(file);
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
	unsigned char *file_buf = new unsigned char[file_size];

	sint64 total_read = 0;
	while(total_read < file_size)
	{
		sint64 bytes_read = ReadBytes(file_buf+total_read, 512);
		if(bytes_read == -1)
		{
			std::cout << "Failed read: " << PHYSFS_getLastError() << std::endl;
			return 0;
		}

		total_read += bytes_read;
	}

	return file_buf;
}

std::istream *Filehandle::GetIStream()
{
	unsigned char *cbuf = ReadFile(); // FIXME <-- LIKELY LEAKS
	membuf *sbuf = new membuf((char *)cbuf, (char *)cbuf + Size()); // this too
	Close();
	return new std::istream(sbuf);
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