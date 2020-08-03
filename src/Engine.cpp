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
	const std::string VERSION		= "PreAlpha v1.2";
	const char* PATH_TILE_METADATA	= "res/tile_mdata.data";
	const char* ID_TILESHEET		= "tile_sheet";
	const char* PATH_DEFAULT_MAP	= "level/default.data";
	const u32 BACKGROUND_COLOR = 0x0d0d0d;
	const int GFX_WIDTH		= 480;
	const int GFX_HEIGHT	= 280;
	const double GFX_SCALE	= 2.0;
	const int TEXTBOX_COLS	= 48;
	const int TEXTBOX_ROWS	= 4;
	const int TM_TILESIZE	= 16;
	const double CAM_LERP	= 0.15;
	
	bool running;
	long long updates;
	enum { GM_TITLE, GM_LOADING, GM_NORMAL, GM_NORMAL_TB, GM_TRANSITION, GM_CUTSCENE, GM_CREDITS } gameMode = GM_NORMAL;
	Graphics* graphics;
	Textbox* textbox;
	Tilemap* tilemap;
	Camera* camera;
	Player* player;

	void showTextBox(Textbox::mode_t mode, const char* string)
	{
		textbox->setMode(mode);
		textbox->setMessage(string);
		gameMode = GM_NORMAL_TB;
	}

	void update()
	{
		Keyboard::prepareForPoll();
		SDL_Event event;
		while(SDL_PollEvent(&event) != 0) // Handle events
	 	{
    		if(event.type == SDL_QUIT) running = false;
    		Keyboard::update(event);
		}
		if(gameMode == GM_NORMAL || gameMode == GM_NORMAL_TB)
		{
			camera->setFocusPoint(player->getRectangle().getCenterX(), player->getRectangle().getCenterY());
			camera->update();
		}
		if(gameMode == GM_NORMAL) player->update(tilemap);
		else if(gameMode == GM_NORMAL_TB) 
		{
			textbox->update();
			if(Keyboard::keyPressedThisFrame())
			{
				if(textbox->mode == Textbox::TB_TYPING)
				{
					if(textbox->isComplete()) gameMode = GM_NORMAL;
					else if(textbox->isPaused()) textbox->unPause();
					else textbox->setSpeed(textbox->TEXT_FAST_SPEED);
				}
			} 
		}
	}

	void render()
	{
		graphics->clearBuffer();
		graphics->drawRectangle(0, 0, graphics->renderBuffer->width, graphics->renderBuffer->height, BACKGROUND_COLOR);
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
		graphics = new Graphics(GFX_WIDTH, GFX_HEIGHT, GFX_SCALE, ("Bleak " + VERSION).c_str());
		textbox = new Textbox(Textbox::TB_TYPING);
		textbox->setDimensions(TEXTBOX_ROWS, TEXTBOX_COLS, graphics->CHARACTER_WIDTH, graphics->CHARACTER_HEIGHT);
		tilemap = new Tilemap(TM_TILESIZE, ID_TILESHEET, PATH_TILE_METADATA);
		tilemap->loadData(PATH_DEFAULT_MAP);
		player = new Player(50, 20);	
		camera = new Camera(graphics->bufferWidth / 2, graphics->bufferHeight / 2);
		camera->setMode(Camera::CAM_FOCUSPOINT);
		camera->setLerp(CAM_LERP);
		camera->setFocusPoint(player->getRectangle().getCenterX(), player->getRectangle().getCenterY());
		showTextBox(Textbox::TB_TYPING, "Welcome to Bleak PreAlpha v1.2!>\nThe project is still in it's "
			"early stages, but it's coming along nicely.>^I'm excited to see how far I will take this engine!\n"
			"This text is going to overflow because it is going on for such an incredible amount of time. These "
			"words are so boring to read I'm surprised you haven't bashed your head into the keyboard, holy crap "
			"I'm getting kinda bored having to type this whole thing out.");
	}

	void exit()
	{
		log("Terminating");
		cleanupTextureAtlas();
		delete graphics;
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
