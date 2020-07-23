#include "Animation.hpp"

Animation::Animation(std::vector<Texture*> frames, int ticksPerFrame): frame(0), nFrames((int) frames.size()), ticks(0), 
	ticksPerFrame(ticksPerFrame)
{
	textures = new Texture*[frames.size()];
    for (int i = 0; i < nFrames; i++) textures[i] = frames.at(i);
}

Animation::~Animation()
{
	delete[] textures;
}

Animation* Animation::getCopy()
{
	std::size_t size = sizeof(*this);
	Animation* copy = (Animation*) malloc(size);
	memcpy(copy, this, size);
	return copy;
}

Texture* Animation::getFrame(int index)
{ 
	if(index < nFrames) return textures[index];
	else return textures[0];
}; 

void Animation::update()
{
	ticks++;
	if(ticks >= ticksPerFrame)
	{
		ticks = 0;
		frame++;
		if(frame >= nFrames) frame = 0;
	}
}

void Animation::setFrame(int frame) { this->frame = frame; ticks = 0; }
Texture* Animation::getCurrentFrame() { return textures[frame]; };
int Animation::getFrameIndex() { return frame; }
