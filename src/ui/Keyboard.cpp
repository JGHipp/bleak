#include "Keyboard.hpp"

Keyboard::Keyboard()
{}

void Keyboard::update(SDL_Event& event)
{
	if(event.type == SDL_KEYDOWN) keys[event.key.keysym.sym] = true;
    else if(event.type == SDL_KEYUP)  keys[event.key.keysym.sym] = false;
}

bool Keyboard::keyPressed(SDL_Keycode key) { return keys[key]; }
