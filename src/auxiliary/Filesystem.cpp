#include <iostream>
#include <vector>
#include <sys/stat.h>
#ifdef _WIN32
	#include <Shlobj.h>
#endif

#include <physfs.h>

#include "Filesystem.h"

// Valid file extensions for data packages
const std::string BASE_DATA_LOCATION = "data"; // We always load data from here
const std::string PACKAGE_EXTENSION = "zip";
const std::string APP_NAME = "spoon"; // FIXME pass from app

std::string fs_userdir;

int Filesystem::Initialise(char *path)
{
	if(!PHYSFS_init(path))
	{
		std::cout << "Error: Filesystem::initialise: " << PHYSFS_getLastError() << std::endl;
		return 0;
	}

	/* Create user directory if required */
	std::string userdir = GetUserDir();
	if(!FSIsDirectory(userdir))
	{
		std::cout << "Error: Filesystem::initialise: Could not create user directory " << userdir << std::endl;
		return 0;
	}

	if(!PHYSFS_setWriteDir(userdir.c_str()))
	{
		std::cout << "Error: Filesystem::initialise: Could not set write dir: " << PHYSFS_getLastError() << std::endl;
		return 0;
	}

	PrintDebugInfo();

	VFSRegisterDirectory(BASE_DATA_LOCATION); // We always register the "base" dir
	//vfsRegisterDirectory(MOD_DATA_LOCATION); // Base dir for the loaded game module

	FSCreateDirectory(userdir + "/data");

	LoadPackages();

	return 1;
}

void Filesystem::Shutdown()
{
	PHYSFS_deinit();
}

std::vector<std::string> Filesystem::ListDirectory(const std::string &path, bool filter_archives)
{
	std::vector<std::string> list;
	char **files = PHYSFS_enumerateFiles(path.c_str());
	for(char **i=files; *i != NULL; i++)
	{
		std::string filename = *i;
		if(filter_archives)
		{
			if(filename.substr(filename.find_last_of(".") + 1) != PACKAGE_EXTENSION)
				list.push_back(filename);
		}
		else
			list.push_back(filename);
	}

	PHYSFS_freeList(files);
	return list;
}

int Filesystem::FileExists(const std::string &path)
{
	return PHYSFS_exists(path.c_str());
}

std::string Filesystem::GetPathSeparator()
{
#ifdef _WIN32
	return "\\";
#endif

	return "/";
}

std::string Filesystem::GetUserDir()
{
	if(!fs_userdir.empty())
		return fs_userdir;

	std::string path;

#ifdef _WIN32
	char wpath[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, wpath);
	path += wpath;
	path += "\\My Games\\";
	path += APP_NAME;
#else
	// On sensible operating systems we use the $HOME variable
	path = getenv("HOME");
	path += "/." + APP_NAME;
#endif

	// create dir if required
	if(!FSIsDirectory(path))
		if(!FSCreateDirectory(path))
			std::cout << "Filesystem::getUserDir: Could not create Directory: " << path << std::endl;

	fs_userdir = path;
	return path;
}

void Filesystem::PrintDebugInfo()
{
	/* Print version */
	PHYSFS_Version version;
	PHYSFS_VERSION(&version);
	std::cout << "PhysFS Version " << (int)version.major << "." << (int)version.minor << "." << (int)version.patch << std::endl;

	/* Print supported archives */
	std::cout << "PhysFS Archive support: ";
	const PHYSFS_ArchiveInfo **i;
	for (i = PHYSFS_supportedArchiveTypes(); *i != NULL; i++)
		std::cout << (*i)->extension << " ";
	std::cout << std::endl;
}

void Filesystem::LoadPackages()
{
	std::vector<std::string> files = ListDirectory("/", false);

	for(std::string filename : files)
	{
		std::string extension = filename.substr(filename.find_last_of(".") + 1);
		if(extension == PACKAGE_EXTENSION)
		{
			std::string full_path(PHYSFS_getRealDir(filename.c_str()));
			full_path = full_path + PHYSFS_getDirSeparator() + filename;

			if(!PHYSFS_mount(full_path.c_str(), "/", true))
				std::cout << "Error: Filesystem::initialise: Could not mount " << full_path << ": " << PHYSFS_getLastError() << std::endl;
			else
				std::cout << "Loaded archive: " << full_path << std::endl;
		}
	}
}

// registers a directory as /
int Filesystem::VFSRegisterDirectory(const std::string &path)
{
	// first we mount from userdir
	std::string userpath = GetUserDir();
	userpath += GetPathSeparator() + path;
	PHYSFS_mount(userpath.c_str(), "/", true);

	// then mount from basedir
	std::string basepath(PHYSFS_getBaseDir());
	basepath += path;

	if(!PHYSFS_mount(basepath.c_str(), "/", true))
	{
		std::cout << "Failed to mount: " << basepath << std::endl;
		return 0;
	}

	return 1;
}


int Filesystem::FSCreateDirectory(const std::string &path)
{
#ifdef _WIN32
	if(mkdir(path.c_str()) != 0)
		return 0;
#else
	if(mkdir(path.c_str(), 0777) != 0)
		return 0;
#endif

	return 1;
}

int Filesystem::FSIsDirectory(const std::string &path)
{
	struct stat info;
	if(stat(path.c_str(), &info) != 0) // cannot access
		return 0;

	if(!(info.st_mode & S_IFDIR)) // not a directory
		return 0;

	return 1;
}