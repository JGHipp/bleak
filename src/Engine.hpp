#ifndef Engine_H
#define Engine_H

#include "level/Level.hpp"
#include "level/Player.hpp"

namespace Engine
{
	extern Level* currentLevel;
	extern Player* player;
	extern long long unsigned int milisecondsSinceEpoch();
	extern void run(int targetFPS);
	extern void exit();
	extern long long updatesSinceStart();	
};

#endif
