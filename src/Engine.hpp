#ifndef Engine_H
#define Engine_H

#include "ui/Keyboard.hpp"
#include "gfx/Graphics.hpp"
#include "level/Tilemap.hpp"
#include "level/Camera.hpp"
#include "level/Player.hpp"

class Engine
{
	private:
		bool running;
		double targetFPS;
		long long updates;
		Graphics* graphics;
		Keyboard* keyboard;
		Tilemap* tilemap;
		Camera* camera;
		Player* player;
		void update();
		void render();
		void init();
		long long unsigned int milisecondsSinceEpoch();
	public:
		Engine(double targetFPS);
		void run();
		void exit();
		long long updatesSinceStart();	
};

#endif
