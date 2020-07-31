#include "Animation.hpp"

Animation::Animation(std::vector<Texture*> frames, int ticksPerFrame): nFrames((int) frames.size()), ticksPerFrame(ticksPerFrame)
{
	textures = new Texture*[frames.size()];
    for (int i = 0; i < nFrames; i++) textures[i] = frames.at(i);
}

Animation::Animation(const Animation& original): frame(original.frame), nFrames(original.nFrames), ticks(original.ticks),
	ticksPerFrame(original.ticksPerFrame), textures(original.textures)
{}

Animation::~Animation()
{
	delete[] textures;
}

Animation* Animation::getCopy()
{
	return new Animation(*this);
}

Texture* Animation::getFrame(int index)
{ 
	if(index < nFrames) return textures[index];
	else return textures[0];
}; 

void Animation::update()
{
	if(halting && haltMode == HALT_IMMEDIATE)
	{
		stopped = true;
		halting = false;
	}
	if(!stopped) ticks++;
	if(ticks >= ticksPerFrame)
	{
		ticks = 0;
		frame++;
		if(frame >= nFrames) frame = 0;
		if(halting && haltMode == HALT_WAIT_NEXT) 
		{
			stopped = true;
			halting = false;
		}
	}
}

void Animation::halt(halt_t type) 
{
	haltMode = type;
	halting = true;
}

bool Animation::isHalted() { return stopped; }
void Animation::unHalt() { stopped = false; }
void Animation::setFrame(int frame) { this->frame = frame; ticks = 0; }
Texture* Animation::getCurrentFrame() { return textures[frame]; };
int Animation::getFrameIndex() { return frame; }
