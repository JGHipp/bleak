#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <cstddef>
#include <SDL2/SDL.h>

namespace Keyboard
{
	extern bool keyPressedThisFrame();
	extern bool keyPressed(SDL_Keycode key);
	extern void prepareForPoll();
	extern void update(SDL_Event& event);
};

#endif
