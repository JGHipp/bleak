#include "Textbox.hpp"

Textbox::Textbox(mode_t mode): mode(mode)
{}

Textbox::~Textbox()
{}

void Textbox::update()
{	
	if(mode == TB_TYPING)
	{
		if(updates >= framesPerTick)
		{
			updates = 0;
			if(nChars < (int) buffer.length()) nChars++;
		}
		updates++;
	}
}

void Textbox::render(Graphics* graphics)
{
	std::string text = (mode == TB_TYPING ? buffer.substr(0, nChars): buffer);
	graphics->drawText(text, 0, 0, 0xFF);
}

void Textbox::setBuffer(std::string buffer){ this->buffer = buffer; }
void Textbox::setMode(mode_t mode){ this->mode = mode; }
void Textbox::setFramesPerTick(int fpt){ framesPerTick = fpt; }
void Textbox::setNCharsShown(int nChars){ this->nChars = nChars; }

