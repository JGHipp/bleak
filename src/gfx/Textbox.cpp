/*
	Textbox message special characters:
	'\n': Newline
	'>'	: Pause (displays a blinker)
	'^' : Clear the textbox
*/

#include "Textbox.hpp"
#include <iostream>
#include <algorithm>
#include <vector>

Textbox::Textbox(mode_t mode): mode(mode)
{}

Textbox::~Textbox()
{}

void Textbox::advanceChar()
{
	if(++nChars > (int) buffer.length()) return;
	int nNewlines = 0, lastSpace = 0;
	size_t lookAhead = buffer.find_first_of(' ', nChars)  - 1;
	for(int i = 0; i < (int) (lookAhead != std::string::npos && lookAhead < (size_t) buffer.length() ? lookAhead : nChars); i++)
	{
		if(buffer.at(i) == '\n' || buffer.at(i) == WRAP_CHAR) nNewlines++;
		if(buffer.at(i) == ' ') lastSpace = i;
		if(buffer.at(i) == CLEAR_CHAR) nNewlines = 0;
	}
	if(nNewlines > rows - 1)
	{
		buffer.erase(lastSpace, 1);
		buffer.insert(lastSpace, charToString(PAUSE_CHAR) + charToString(CLEAR_CHAR));
		nChars = lastSpace + 1;
	}
	if(nChars > 0 && buffer.at(nChars - 1) == CLEAR_CHAR) // Clear the text box up to that point
	{
		std::string temp = buffer.substr(nChars);
		// Reformat rest of text
		for(int i = 0; i < (int) temp.length(); i++) if(temp.at(i) == WRAP_CHAR) temp.at(i) = ' ';
		buffer = parse(temp);
		nChars = 0;
	}
	if(nChars > 0 && buffer.at(nChars - 1) == PAUSE_CHAR) paused = true;
	if(nChars > 1 && buffer.at(nChars - 2) == PAUSE_CHAR) buffer.erase(nChars-- - 2, 1); // Delete blinkers once past them	
	if(nChars == (int) buffer.length()) complete = true;	
}

void Textbox::update()
{	
	if(mode == TB_TYPING)
	{
		if(cUpdates++ >= TICKS_PER_UPDATE && !paused && !complete)
		{
			cUpdates = 0;
			if(nChars < (int) buffer.length())
				 for(int i = 0; i < textSpeed; i++) if(!paused && !complete) advanceChar();
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
	graphics->drawRectangle(BX_PADDING, graphics->bufferHeight - height - BY_PADDING, width, height, BOX_COLOR);
	graphics->drawRectangleOutline(BX_PADDING, graphics->bufferHeight - height - BY_PADDING, width, height, OUTLINE_COLOR);
	// Replace PAUSE_CHAR with blinker
	std::string truncated = buffer.substr(0, nChars);
	for(int i = 0; i < (int) truncated.length(); i++) 
		if(truncated.at(i) == PAUSE_CHAR) truncated.at(i) = (blinkChar ? BLINK[0] : BLINK[1]);
	// Split buffer into lines
	std::replace(truncated.begin(), truncated.end(), WRAP_CHAR, '\n');
	std::vector<std::string> lines;
    while(truncated.size())
    {
        int index = truncated.find("\n");
       	if(index != (int) std::string::npos)
       	{
           	lines.push_back(truncated.substr(0, index));
           	truncated = truncated.substr(index + 1);
          	if(truncated.size() == 0) lines.push_back(truncated);
        }
        else
        {
           	lines.push_back(truncated);
           	truncated = "";
        }
   	}	
	// Render each line
	for(int i = 0; i < (int) lines.size(); i++)
		graphics->drawText(lines.at(i), BX_PADDING + TX_PADDING, graphics->bufferHeight - height - BY_PADDING + 
			TY_PADDING + i * (graphics->CHARACTER_HEIGHT + LINE_PADDING), TEXT_COLOR);
}

std::string Textbox::parse(std::string message)
{
	std::string buffer = message;
	// Wrap lines if exceeding certain width, but only by full words
	if(buffer.length() > 0)
	{
		int lastSpace = 0, charsOnLine = 0;
		for(int i = 0; i < (int) buffer.length(); i++)
		{
			char character = buffer.at(i);
			if(character == '\n')
			{
				charsOnLine = 0;	
			} 
			else if(charsOnLine++ > cols)
			{
				buffer.at(lastSpace) = WRAP_CHAR;
				charsOnLine = i - lastSpace;			
			}
			if(character == ' ') lastSpace = i;
		}
	}
	return buffer;
}

void Textbox::setMessage(std::string message)
{
	this->originalMessage = message; 
	this->buffer = parse(message + PAUSE_CHAR);
	paused = false;
	complete = false;
	blinkChar = 0;
	cUpdates = 0;
	bUpdates = 0;
	nChars = 0;
}

void Textbox::setDimensions(int rows, int cols, int charWidth, int charHeight)
{
	this->width = TX_PADDING * 2 + cols * charWidth;
	this->height = TY_PADDING * 2 + rows * charHeight + (rows - 1) * LINE_PADDING;
	this->cols = cols;
	this->rows = rows;
}

void Textbox::unPause()
{
	paused = false; 
	textSpeed = TEXT_DEFAULT_SPEED;
}

void Textbox::setMode(mode_t mode){ this->mode = mode; }
void Textbox::setSpeed(int textSpeed) { this->textSpeed = textSpeed; }
void Textbox::setNCharsShown(int nChars){ this->nChars = nChars; }
bool Textbox::isComplete(){ return (mode == TB_STATIC || complete); }
bool Textbox::isPaused() { return paused; }
