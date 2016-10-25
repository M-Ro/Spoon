#include <iostream>
#include <fstream>
#include <sstream>

#include <stdio.h>

#include "Config.h"
#include "Functions.h"

bool Config::cfgLoaded;
std::map<std::string, Config::ConfigVar> Config::cvars;
std::map<std::string, Uint8> Config::keybinds;
std::map<std::string, Uint8> Config::mousebinds;

enum Section { None, Settings, Keybinds };

std::string Config::GetStr(const std::string &key)
{
	if(!cfgLoaded)
		Reload();

	if(cvars.count(key))
		return cvars[key].str;
	else
		return std::string("");
}

float Config::GetFloat(const std::string &key)
{
	if(!cfgLoaded)
		Reload();

	if(cvars.count(key))
		return cvars[key].fval;
	else
		return 0;
}

int Config::GetInt(const std::string &key)
{
	if(!cfgLoaded)
		Reload();

	if(cvars.count(key))
		return cvars[key].ival;
	else
		return 0;
}

Uint8 Config::GetKey(const std::string &key)
{
	if(!cfgLoaded)
		Reload();

	if(mousebinds.count(key))
		return mousebinds[key];

	if(keybinds.count(key))
		return keybinds[key];
	else
		return SDLK_UNKNOWN;
}

// FIXME use currently loaded game/mod folder
// FIXME This should not bypass virtual file system. Userdir should be mounted
// and cfg loaded as Filehandle. ::WriteConfig() same issue
void Config::Reload()
{
	std::string cfgPath = Filesystem::GetUserDir();
	cfgPath += "/data/config.cfg";

	std::ifstream file;
	file.open(cfgPath, std::ios::in);
	if(!file.is_open())
	{
		cfgPath = "data/default.cfg";
		file.open(cfgPath, std::ios::in);
	}

	if(!file.is_open())
	{
		std::cout << "Error: Could not load default config data/default.cfg" << std::endl;
		cfgLoaded = true;
		return;
	}

	std::cout << "Loaded Config: " << cfgPath << std::endl;

	Section section;

	std::string line;
	while(getline(file, line))
	{
		Trim(line);

		if(line == "")
			continue;

		if(line == "[Settings]")
		{
			section = Settings;
			continue;
		}
		else if(line == "[Keybinds]")
		{
			section = Keybinds;
			continue;
		}

		std::stringstream ss(line);
		std::string key;
		std::string val;

		getline(ss, key, '=');
		getline(ss, val, '=');

		key = Trim(key);
		val = Trim(val);

		if(section == Settings)
		{
			std::stringstream sval(val);

			ConfigVar cvar = ConfigVar();
			cvar.str = val;
			sval >> cvar.fval;
			sval.seekg(0, std::ios::beg);
			sval >> cvar.ival;

			cvars[key] = cvar;
		}
		else if(section == Keybinds)
		{
			if(key.find("mouse") != std::string::npos)
			{
				Uint8 btn = key[5] - '0';
				mousebinds[val] = btn;
			}
			else
			{
				Uint8 keyCode = SDL_GetKeyFromName(key.c_str());
				if(keyCode == SDLK_UNKNOWN)
					std::cout << "Warning: Keybind invalid: " << key << std::endl;
				else
					keybinds[val] = keyCode;
			}
		}
	}

	file.close();
	cfgLoaded = true;
}

void Config::WriteConfig()
{
	std::string cfgUserPath = Filesystem::GetUserDir();
	cfgUserPath += "/data/config.cfg";

	std::ofstream file;
	file.open(cfgUserPath, std::ios::out);
	if(!file.is_open())
	{
		std::cout << "Error: Could not open " << cfgUserPath << " for writing." << std::endl;
		return;
	}

	/* Save Settings Section */
	file << "[Settings]" << std::endl;
	for (auto& kv : cvars)
		file << kv.first << "=" << kv.second.str << std::endl;

	/* Save Keybinds Section */
	file << "[Keybinds]" << std::endl;
	for (auto& kv : keybinds)
		file << SDL_GetKeyName(kv.second) << "=" << kv.first << std::endl;
	for (auto& kv : mousebinds)
		file << "mouse" << (int)(kv.second) << "=" << kv.first << std::endl;

	file.close();
}