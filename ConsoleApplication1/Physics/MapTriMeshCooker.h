#pragma once

#ifdef _WIN32
#include <windows.h>
#endif

#include "../Render/Mapfile.h"

class MapTriMeshCooker
{
public:
	static void BuildMapPhysicsMesh(Mapfile &map);
};