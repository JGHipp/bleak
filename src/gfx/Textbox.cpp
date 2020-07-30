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
	graphics->drawRectangle(BX_PADDING, graphics->bufferHeight - BOX_HEIGHT - BY_PADDING, graphics->bufferWidth - BX_PADDING * 2, 
		BOX_HEIGHT, BOX_COLOR);
	graphics->drawRectangleOutline(BX_PADDING, graphics->bufferHeight - BOX_HEIGHT - BY_PADDING, graphics->bufferWidth - BX_PADDING * 2, 
		BOX_HEIGHT, OUTLINE_COLOR);
	graphics->drawText(text, BX_PADDING + TX_PADDING, graphics->bufferHeight - BOX_HEIGHT - BY_PADDING + TY_PADDING, TEXT_COLOR);
}

void Textbox::setBuffer(std::string buffer){ this->buffer = buffer; }
void Textbox::setMode(mode_t mode){ this->mode = mode; }
void Textbox::setFramesPerTick(int fpt){ framesPerTick = fpt; }
void Textbox::setNCharsShown(int nChars){ this->nChars = nChars; }

