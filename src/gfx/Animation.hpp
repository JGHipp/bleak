#ifndef ANIMATION_H
#define ANIMATION_H

#include "Texture.hpp"
#include <vector>

class Animation
{
	private:
		bool stopped = false, halting = false;
		int frame = 0, nFrames = 0, ticks = 0, ticksPerFrame = 0;
		Texture** textures;
	public:
		enum halt_t { HALT_IMMEDIATE, HALT_WAIT_NEXT} haltMode;
		Animation(std::vector<Texture*> textures, int ticksPerFrame);
		Animation(const Animation& original); // Copy constructor
		~Animation();
		Animation* getCopy();
		Texture* getCurrentFrame();	
		Texture* getFrame(int index);
		void update();
		void setFrame(int frame);
		int getFrameIndex();
		void halt(halt_t type);
		bool isHalted();
		void unHalt();
};

#endif
