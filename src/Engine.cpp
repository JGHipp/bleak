/*
	Written by John Gustav Hippisley
*/
#include "Engine.hpp"
#include <iostream>
#include <string>		// std::string
#include <chrono> 		// std::chrono
#ifdef __MINGW32__		// Add suport for cross-compiling
#include <mingw.thread.h>
#else
#include <thread>		// std::this_thread::sleep_for()
#endif
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h> 	// Simple DirectMedia Layer

const std::string VERSION = "PreAlpha v1.0";

Engine::Engine(double targetFPS): targetFPS(targetFPS), updates(0)
{
	log("Bleak %s running at %i FPS", VERSION.c_str(), (int) targetFPS);
}

void Engine::run()
{
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

void Engine::update()
{
	SDL_Event event;
	while(SDL_PollEvent(&event) != 0) // Handle events
 	{
    	if(event.type == SDL_QUIT) running = false;
    	keyboard->update(event);
	}
	player->update(keyboard, tilemap);
	camera->setFocusPoint(player->getRectangle().getCenterX(), player->getRectangle().getCenterY());
	camera->update();
}

void Engine::render()
{
	graphics->clearBuffer();
	graphics->drawRectangle(0, 0, graphics->renderBuffer->width, graphics->renderBuffer->height, 0x0d0d0d);
	tilemap->render(graphics, camera, TILE_BACKGROUND);
	player->render(graphics, camera);
	tilemap->render(graphics, camera, TILE_FOREGROUND);
	graphics->updateWindow();
}

void Engine::init()
{
	running = true;
	SDL_Init(SDL_INIT_EVERYTHING);
	initTextureAtlas(); // Load all textures and animations
	keyboard = new Keyboard();
	graphics = new Graphics(480, 280, 2.0, ("Bleak " + VERSION).c_str());
	tilemap = new Tilemap(16, "res/tilesheet.bmp", "res/tile_mdata.data");
	tilemap->loadData("level/default.data");
	camera = new Camera(graphics->renderBuffer->width / 2, graphics->renderBuffer->height / 2);
	camera->setMode(2);
	camera->setLerp(0.15);
	player = new Player(20, 20);	
}

void Engine::exit()
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

long long Engine::updatesSinceStart() { return updates; }
unsigned long long Engine::milisecondsSinceEpoch()
{ return std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1); }

