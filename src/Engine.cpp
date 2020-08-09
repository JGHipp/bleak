/*
	Written by John Gustav Hippisley
*/
#include "Engine.hpp"
#include "util/Log.hpp"
#include "gfx/Graphics.hpp"
#include "gfx/Textbox.hpp"
#include "gfx/Camera.hpp"
#include "ui/Keyboard.hpp"
#include <iostream>
#include <string>
#include <chrono>
#ifdef __MINGW32__
#include <mingw.thread.h>
#else
#include <thread>
#endif
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

namespace Engine
{
	// Constants
	const std::string VERSION		= "PreAlpha v1.3";
	const char* PATH_DEFAULT_MAP	= "level/default.data";
	const u32 BACKGROUND_COLOR = 0x0d0d0d;
	const int GFX_WIDTH		= 480;
	const int GFX_HEIGHT	= 380;
	const double GFX_SCALE	= 2.0;
	const int TEXTBOX_COLS	= 48;
	const int TEXTBOX_ROWS	= 4;
	const double CAM_LERP	= 0.055;
	
	bool running;
	long long updates;
	enum { GM_TITLE, GM_LOADING, GM_NORMAL, GM_NORMAL_TB, GM_TRANSITION, GM_CUTSCENE, GM_CREDITS } gameMode = GM_NORMAL;
	Level* currentLevel;
	Player* player;

	void showTextBox(Textbox::mode_t mode, const char* string)
	{
		Textbox::mode = mode;
		Textbox::setMessage(string);
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
		if(gameMode == GM_NORMAL) player->update(currentLevel->tilemap);
		else if(gameMode == GM_NORMAL_TB) 
		{
			Textbox::update();
			if(Keyboard::keyPressedThisFrame())
			{
				if(Textbox::mode == Textbox::TB_TYPING)
				{
					if(Textbox::isComplete()) gameMode = GM_NORMAL;
					else if(Textbox::isPaused()) Textbox::unPause();
					else Textbox::textSpeed = Textbox::TEXT_FAST_SPEED;
				}
			} 
		}
		if(gameMode == GM_NORMAL || gameMode == GM_NORMAL_TB)
		{
			Camera::setFocusPoint((int) player->getX() + PLAYER_WIDTH / 2, (int) player->getY() + PLAYER_HEIGHT / 2);
			Camera::update();
		}
	}

	void render()
	{
		Graphics::clearBuffer();
		Graphics::drawRectangle(0, 0, Graphics::bufferWidth, Graphics::bufferHeight, BACKGROUND_COLOR);
		if(gameMode == GM_NORMAL || gameMode == GM_NORMAL_TB)
		{
			currentLevel->render(Level::LEVEL_BACKGROUND);
			player->render();
			currentLevel->render(Level::LEVEL_FOREGROUND);
		}
		if(gameMode == GM_NORMAL_TB) Textbox::render();
		Graphics::updateWindow();
	}	

	void init()
	{
		running = true;
		SDL_Init(SDL_INIT_EVERYTHING);
		initTextureAtlas(); // Load all textures and animations
		Graphics::init(GFX_WIDTH, GFX_HEIGHT, GFX_SCALE, ("Bleak " + VERSION).c_str());
		currentLevel = new Level(PATH_DEFAULT_MAP);
		player = new Player(300, 272);	
		Camera::setCenter(Graphics::bufferWidth / 2, Graphics::bufferHeight / 2);
		Camera::setFocusPoint(player->getRectangle().getCenterX(), player->getRectangle().getCenterY());
		Camera::mode = Camera::CAM_FOCUSPOINT;
		Camera::lerp = CAM_LERP;
		Textbox::setDimensions(TEXTBOX_ROWS, TEXTBOX_COLS, Graphics::CHARACTER_WIDTH, Graphics::CHARACTER_HEIGHT);
		showTextBox(Textbox::TB_TYPING, "Welcome to Bleak PreAlpha v1.2!>\nSo far this is only a basic platforming "
		"engine, but there will be more added! :)");
	}

	void exit()
	{
		log("Terminating");
		cleanupTextureAtlas();
		delete currentLevel;
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
