#include "Keyboard.hpp"
#include <iostream>
#include <vector>
namespace Keyboard
{
	std::vector<SDL_Keycode> keyHash;
	std::vector<bool> keyValues;
	bool keyPress = false;

	void prepareForPoll()
	{
		keyPress = false;
	}

	void setField(SDL_Keycode key, bool pressed)
	{
		bool found = false;
		for(int i = 0; i < (int) keyHash.size(); i++)
		{
			if(keyHash.at(i) == key) 
			{
				found = true;
				keyValues.at(i) = pressed;			
			}
		}
		if(!found)
		{
			keyHash.push_back(key);
			keyValues.push_back(pressed);
		}
	}

	void update(SDL_Event& event)
	{
		if(event.type == SDL_KEYDOWN)
		{
			keyPress = true;	
			setField(event.key.keysym.sym, true);
		} 
   	 	else if(event.type == SDL_KEYUP) setField(event.key.keysym.sym, false);
	}

	bool keyPressed(SDL_Keycode key) 
	{
		bool pressed = false;
		for(int i = 0; i < (int) keyHash.size(); i++)
			if(keyHash.at(i) == key && keyValues.at(i)) pressed = true;	
		return pressed; 
	}
	
	bool keyPressedThisFrame() { return keyPress; }
}
