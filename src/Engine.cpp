/*
	Written by John Gustav Hippisley
*/
#include "Engine.hpp"
#include <iostream>
#include <string>
#include <chrono>
#ifdef __MINGW32__		// Add suport for cross-compiling
#include <mingw.thread.h>
#else
#include <thread>		// std::this_thread::sleep_for()
#endif
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h> 	// Simple DirectMedia Layer

namespace Engine
{
	// Constants
	const std::string VERSION		= "PreAlpha v1.1";
	const char* PATH_TILE_METADATA	= "res/tile_mdata.data";
	const char* PATH_TILESHEET		= "res/tilesheet.bmp";
	const char* PATH_DEFAULT_MAP	= "level/default.data";
	const int GFX_WIDTH		= 480;
	const int GFX_HEIGHT	= 280;
	const double GFX_SCALE	= 2.0;
	const int TM_TILESIZE	= 16;
	const double CAM_LERP	= 0.15;
	
	bool running;
	long long updates;
	enum { GM_TITLE, GM_LOADING, GM_NORMAL, GM_NORMAL_TB, GM_TRANSITION, GM_CUTSCENE, GM_CREDITS } gameMode = GM_NORMAL;
	Graphics* graphics;
	Textbox* textbox;
	Keyboard* keyboard;
	Tilemap* tilemap;
	Camera* camera;
	Player* player;

	void update()
	{
		SDL_Event event;
		while(SDL_PollEvent(&event) != 0) // Handle events
	 	{
    		if(event.type == SDL_QUIT) running = false;
    		keyboard->update(event);
		}
		if(gameMode == GM_NORMAL)
		{
			player->update(keyboard, tilemap);
			camera->setFocusPoint(player->getRectangle().getCenterX(), player->getRectangle().getCenterY());
			camera->update();
		}
		else if(gameMode == GM_NORMAL_TB) textbox->update();
	}

	void render()
	{
		graphics->clearBuffer();
		graphics->drawRectangle(0, 0, graphics->renderBuffer->width, graphics->renderBuffer->height, 0x0d0d0d);
		if(gameMode == GM_NORMAL || gameMode == GM_NORMAL_TB)
		{
			tilemap->render(graphics, camera, Tilemap::TILE_BACKGROUND);
			player->render(graphics, camera);
			tilemap->render(graphics, camera, Tilemap::TILE_FOREGROUND);
		}
		if(gameMode == GM_NORMAL_TB) textbox->render(graphics);
		graphics->updateWindow();
	}

	void init()
	{
		running = true;
		SDL_Init(SDL_INIT_EVERYTHING);
		initTextureAtlas(); // Load all textures and animations
		keyboard = new Keyboard();
		graphics = new Graphics(GFX_WIDTH, GFX_HEIGHT, GFX_SCALE, ("Bleak " + VERSION).c_str());
		textbox = new Textbox(Textbox::TB_STATIC);
		textbox->setBuffer("The quick brown fox jumped over the lazy dog. 0123456789!@#$%^&*()");
		tilemap = new Tilemap(TM_TILESIZE, PATH_TILESHEET, PATH_TILE_METADATA);
		tilemap->loadData(PATH_DEFAULT_MAP);
		player = new Player(20, 20);	
		camera = new Camera(graphics->bufferWidth / 2, graphics->bufferHeight / 2);
		camera->setMode(Camera::CAM_FOCUSPOINT);
		camera->setLerp(CAM_LERP);
		camera->setFocusPoint(player->getRectangle().getCenterX(), player->getRectangle().getCenterY());
	}

	void exit()
	{
		log("Terminating");
		cleanupTextureAtlas();
		delete graphics;
		delete keyboard;
		delete tilemap;
		delete camera;
		delete player;
		SDL_Quit();
	}

	long long updatesSinceStart() { return updates; }
	unsigned long long milisecondsSinceEpoch() 
	{ return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1); }

	void run(int targetFPS)
	{
		updates = 0;
		log("Bleak %s running at %i FPS", VERSION.c_str(), targetFPS);
		init();
		double milisecondsPerFrame = 1000 / targetFPS;
		unsigned long long startTime, sleepFor;
		while(running)
		{
			startTime = milisecondsSinceEpoch();
			update();
			render();
			updates++;
			sleepFor = milisecondsPerFrame - (milisecondsSinceEpoch() - startTime);
			// Sleep for time neccesary to run at target FPS
			if(sleepFor > 0) std::this_thread::sleep_for(std::chrono::milliseconds(sleepFor));
		}
		exit();
	}
}
