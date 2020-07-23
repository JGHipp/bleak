#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <cstddef>	// size_t
#include <SDL2/SDL.h>

class Keyboard
{
	private:
		const static size_t N_KEYS = pow(2, sizeof(SDL_Keycode) * 8) - 1 ;
		bool keys[N_KEYS];
	public:
		Keyboard();
		bool keyPressed(SDL_Keycode key);
		void update(SDL_Event& event);
};

#endif
