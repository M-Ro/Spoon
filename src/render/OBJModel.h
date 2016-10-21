#pragma once

#include <string>

#include "Model.h"

namespace OBJModel
{
	Model *Load(std::string const &filepath);
}