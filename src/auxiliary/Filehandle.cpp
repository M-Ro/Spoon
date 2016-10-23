#include "Filehandle.h"

Filehandle::Filehandle(const std::string &path, bool readonly)
{
	file = nullptr;
	open(path, readonly);
}

Filehandle::~Filehandle()
{

}

int Filehandle::open(const std::string &path, bool readonly)
{
	if(file != nullptr)
		close();

	if(!readonly)
		file = PHYSFS_openWrite(path.c_str());
	else
		file = PHYSFS_openRead(path.c_str());

	if(file == nullptr)
		return 0;

	return 1;
}

int Filehandle::close()
{
	return PHYSFS_close(file);
}

sint64 Filehandle::read(void *buffer, uint32 objsize, uint32 objcount)
{
	return PHYSFS_read(file, buffer, objsize, objcount);
}

sint64 Filehandle::readBytes(void *buffer, uint32 count)
{
	return PHYSFS_read(file, buffer, 1, count);
}

sint64 Filehandle::tell()
{
	return PHYSFS_tell(file);
}

int Filehandle::seek(uint64 pos)
{
	return PHYSFS_seek(file, pos);
}

int Filehandle::eof()
{
	return PHYSFS_eof(file);
}

sint64 Filehandle::size()
{
	return PHYSFS_fileLength(file);
}

bool Filehandle::is_open()
{
	if(file == nullptr)
		return false;
	if(tell() == -1)
		return false;

	return true;
}