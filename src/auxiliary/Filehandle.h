#pragma once

#include <string>
#include <istream>
#include <streambuf>

#include <physfs.h>

const int FILE_MAXREAD = 512;

typedef long long 			sint64;
typedef int 				sint32;
typedef unsigned long long 	uint64;
typedef unsigned int 		uint32;

struct membuf : std::streambuf
{
    membuf(char* begin, char* end) {
        this->setg(begin, begin, end);
    }
};

// Behaves like a C FILE, wraps PHYSFS_file functions
class Filehandle
{
public:

	Filehandle(const std::string &path, bool readonly = true);
	~Filehandle();

	int Open(const std::string &path, bool readonly = true);

	int Close();

	sint64 ReadBytes(void *buffer, uint32 count);

	// Attempts to read the entire file and returns the buffer
	unsigned char *ReadFile();

	std::istream *GetIStream();

	sint64 Tell();

	int Seek(uint64 pos);

	int Eof();

	sint64 Size();

	bool IsOpen();

private:

	PHYSFS_File *file;
};