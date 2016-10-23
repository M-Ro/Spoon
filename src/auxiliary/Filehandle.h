#pragma once

#include <string>

#include <physfs.h>

typedef long long 			sint64;
typedef int 				sint32;
typedef unsigned long long 	uint64;
typedef unsigned int 		uint32;

// Behaves like a C FILE, wraps PHYSFS_file functions
class Filehandle
{
public:

	Filehandle(const std::string &path, bool readonly = true);
	~Filehandle();

	int open(const std::string &path, bool readonly = true);

	int close();

	sint64 read(void *buffer, uint32 objsize, uint32 objcount);

	sint64 readBytes(void *buffer, uint32 count);

	sint64 tell();

	int seek(uint64 pos);

	int eof();

	sint64 size();

	bool is_open();

private:

	PHYSFS_File *file;
};