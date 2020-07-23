#ifndef ANIMATION_H
#define ANIMATION_H

#include "Texture.hpp"
#include <vector> // std::vector

class Animation
{
	private:
		int frame, nFrames, ticks, ticksPerFrame;
		Texture** textures;
	public:
		Animation(std::vector<Texture*> textures, int ticksPerFrame);
		~Animation();
		Animation* getCopy();
		Texture* getCurrentFrame();	
		Texture* getFrame(int index);
		int getFrameIndex();
		void setFrame(int frame);
		void update();
};

#endif
