#pragma once

#include <string>
#include <map>
#include <list>

#include <SDL2/SDL.h>

class InputHandler
{
public:
	InputHandler();
	~InputHandler();

	void Update();

	int GetMouseX() { return mx; }
	int GetMouseY() { return my; }

	bool KeyDown(int key);

	bool KeyPressed(int key);

	bool MouseDown(Uint8 button);

	bool MousePressed(Uint8 button);

	//string GetTextInput()

	void FlushTextBuffer() { textBuffer = ""; }

	void SetRelativeMouseMode(bool val) { SDL_SetRelativeMouseMode((SDL_bool)val); relMouse = val; }

private:

	bool relMouse;

	int mx;
	int my;

	std::string textBuffer;

	std::map<int, bool> keyDownMap;
	std::map<int, bool> keyPressedMap;
	std::map<Uint8, bool> mouseDownMap;
	std::map<Uint8, bool> mousePressedMap;

	// Custom events e.g SDL_QUIT
	std::list<SDL_EventType> events;

};