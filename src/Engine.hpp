#ifndef Engine_H
#define Engine_H

#include "util/Log.hpp"
#include "ui/Keyboard.hpp"
#include "gfx/Graphics.hpp"
#include "gfx/Textbox.hpp"
#include "level/Level.hpp"
#include "level/Camera.hpp"
#include "level/Player.hpp"

namespace Engine
{
	extern Graphics* graphics;
	extern Textbox* textbox;
	extern Camera* camera;
	extern Level* currentLevel;
	extern Player* player;
	extern long long unsigned int milisecondsSinceEpoch();
	extern void run(int targetFPS);
	extern void exit();
	extern long long updatesSinceStart();	
};

#endif
