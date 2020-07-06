#pragma once

#include "../render/Renderer.h"
#include "../input/InputHandler.h"
#include "AudioInterface.h"
#include "Base/Entity/Entity.h"

extern Renderer* renderer;
extern Camera* camera;
//Config *config;
extern AudioInterface* audio;
extern InputHandler* input;