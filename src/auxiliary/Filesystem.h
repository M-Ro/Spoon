#pragma once

#include <string>
#include <vector>

class Filesystem
{
public:

	/**
	* Initialises the virtual filesystem. This must be called before any
	* another Filesystem operations.
	* @param path Working path. Often argv[0]
	* @return int - 1 on success, 0 on failure
	*/
	static int Initialise(char *path);

	/**
	* Shuts down the virtual file system and deinitializes PhysFS
	* @return (void)
	*/
	static void Shutdown();

	/**
	* Lists all files in the directory at the provided path.
	* @param path Target directory path
	* @param filter_archives Optional. Filters out archive files from the list
	* @return Returns a vector of strings, each string being a filename
	*/
	static std::vector<std::string> ListDirectory(const std::string &path, bool filter_archives = true);

	/**
	* Returns if a file exists in the VFS at the provided path
	* @param path Target file path
	* @return int - non-zero if filename exists. zero otherwise.
	*/
	static int FileExists(const std::string &path);

	/**
	* Gets the OS specific path separator, ie '\\' for windows.
	* @return string - Path separator for current operating system
	*/
	static std::string GetPathSeparator();

	/**
	* Gets the user writing directory for the current platform, for example
	* '~/.%app%'' on linux or '...Documents\\My Games\\%app%' on Windows.
	* This should only be used by code that functions outside of the VFS
	* @return string - Current user directory for the running application
	*/
	static std::string GetUserDir();

private:

	/**
	* Mounts a directory from the system to the VFS as '/' (root)
	* @param path Target directory path
	* @return int - 1 on success, 0 on failure.
	*/
	static int VFSRegisterDirectory(const std::string &path);

	/**
	* Creates a directory on the system filesystem
	* @param path Target directory path
	* @return int - 1 on success, 0 on failure
	*/
	static int FSCreateDirectory(const std::string &path);

	static int FSIsDirectory(const std::string &path);

	static void PrintDebugInfo();

	static void LoadPackages();
};