#include "InputHandler.h"

InputHandler::InputHandler()
{
	SDL_StartTextInput();
}

InputHandler::~InputHandler()
{
	SDL_StopTextInput();
}

void InputHandler::Update()
{
	if(relMouse)
		mx = my = 0;

	SDL_Event e;

	while(SDL_PollEvent(&e) != 0)
	{
		if (e.type == SDL_MOUSEMOTION)
		{
			if(relMouse)
			{
				mx += e.motion.xrel;
				my += e.motion.yrel;
			}
			else
			{
				mx = e.motion.x;
				my = e.motion.y;
			}
		}
		else if(e.type == SDL_MOUSEBUTTONDOWN)
		{
			mouseDownMap[e.button.button] = true;
		}
		else if(e.type == SDL_MOUSEBUTTONUP)
		{
			mouseDownMap[e.button.button] = false;
			mousePressedMap[e.button.button] = true;
		}
		else if(e.type == SDL_KEYDOWN)
		{
			keyDownMap[e.key.keysym.sym] = true;
		}
		else if(e.type == SDL_KEYUP)
		{
			keyDownMap[e.key.keysym.sym] = false;
			keyPressedMap[e.key.keysym.sym] = true;
		}
	}
}

bool InputHandler::KeyDown(int key) // FIXME map doesnt exidst
{
	return keyDownMap[key];
}

bool InputHandler::KeyPressed(int key)
{
	bool pressed = keyPressedMap[key];
	keyPressedMap[key] = false;
	return pressed;
}

bool InputHandler::MouseDown(Uint8 button)
{
	return mouseDownMap[button];
}

bool InputHandler::MousePressed(Uint8 button)
{
	bool pressed = mousePressedMap[button];
	mousePressedMap[button] = false;
	return pressed;
}