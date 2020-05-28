#pragma once

#include <string>
#include <map>

#include <SDL2/SDL.h>

#include "Filesystem.h"

class Config
{
public:

	static std::string GetStr(const std::string &key);

	static float GetFloat(const std::string &key);

	static int GetInt(const std::string &key);

	static Uint8 GetKey(const std::string &key);

	static void Reload();

	static void WriteConfig();

private:

	struct ConfigVar
	{
		std::string str;
		float fval;
		int ival;
	};

	static bool cfgLoaded;
 	static std::map<std::string, ConfigVar> cvars;
 	static std::map<std::string, Uint8> keybinds;
 	static std::map<std::string, Uint8> mousebinds;
};