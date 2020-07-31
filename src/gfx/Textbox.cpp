#include "Textbox.hpp"
#include <iostream>
#include <vector>
Textbox::Textbox(mode_t mode): mode(mode)
{}

Textbox::~Textbox()
{}

void Textbox::update()
{	
	if(mode == TB_TYPING)
	{
		if(cupdates++ >= TEXT_SPEED)
		{
			cupdates = 0;
			if(nChars < (int) buffer.length()) nChars++;
		}
		if(bUpdates++ >= BLINK_SPEED)
		{
			blinkChar = !blinkChar;
			bUpdates = 0;
		}
	}
}

void Textbox::render(Graphics* graphics)
{
	// Draw text box
	const int BOX_WIDTH = graphics->bufferWidth - BX_PADDING * 2;
	graphics->drawRectangle(BX_PADDING, graphics->bufferHeight - BOX_HEIGHT - BY_PADDING, BOX_WIDTH, BOX_HEIGHT, BOX_COLOR);
	graphics->drawRectangleOutline(BX_PADDING, graphics->bufferHeight - BOX_HEIGHT - BY_PADDING, BOX_WIDTH, BOX_HEIGHT, OUTLINE_COLOR);
	const int MAX_CHARS = (BOX_WIDTH - TX_PADDING * 2) / graphics->CHARACTER_WIDTH;
	std::string text = buffer;
	if(isCaughtUp() && mode == TB_TYPING) text.append(std::string(1, blinkChar ? BLINK[0] : BLINK[1]));
	if(text.length() > 0)
	{
		// Insert newlines where words would be split when wrapped
		int lastSpace = 0, charsOnLine = 0;
		for(int i = 0; i < (int) text.length(); i++)
		{
			char character = text.at(i);
			if(character == '\n') charsOnLine = 0;
			if(charsOnLine++ > MAX_CHARS)
			{
				text.at(lastSpace) = '\n';
				charsOnLine = i - lastSpace;
			}
			if(character == ' ') lastSpace = i;
		}
		if(mode == TB_TYPING) text = text.substr(0, nChars + isCaughtUp());
		// Sort text into individual lines
	 	std::vector<std::string> lines;
    	while(text.size())
    	{
        	int index = text.find("\n");
       		if(index != (int) std::string::npos)
       		{
            	lines.push_back(text.substr(0, index));
            	text = text.substr(index + 1);
           		if(text.size() == 0) lines.push_back(text);
        	}
        	else
        	{
            	lines.push_back(text);
            	text = "";
        	}
   		}

		// Render each line
		for(int i = 0; i < (int) lines.size(); i++)
			graphics->drawText(lines.at(i), BX_PADDING + TX_PADDING, graphics->bufferHeight - BOX_HEIGHT - BY_PADDING + 
				TY_PADDING + (graphics->CHARACTER_HEIGHT + LINE_PADDING) * i, TEXT_COLOR);
	}
}

void Textbox::setBuffer(std::string buffer){ this->buffer = buffer; }
void Textbox::setMode(mode_t mode){ this->mode = mode; }
bool Textbox::isCaughtUp(){ return ((mode == TB_STATIC) || (nChars == (int) buffer.length())); }
void Textbox::setNCharsShown(int nChars){ this->nChars = nChars; }
