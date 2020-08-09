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

namespace Textbox
{
	std::string originalMessage, buffer;
	mode_t mode;
	bool paused = false, complete = false;
	int width = 0, height = 0, rows = 0, cols = 0, nChars = 0, cUpdates = 0, bUpdates = 0, blinkChar = 0;
	int textSpeed = TEXT_DEFAULT_SPEED;
	const int BLINK_SPEED = 10, TICKS_PER_UPDATE = 1, TEXT_DEFAULT_SPEED = 2, TEXT_FAST_SPEED = 8;
	const int BX_PADDING = 50, BY_PADDING = 10, TX_PADDING = 10, TY_PADDING = 10, LINE_PADDING = 5;
	const char PAUSE_CHAR = '>', WRAP_CHAR = '\\', CLEAR_CHAR = '^', BLINK[2] = { (char) 219, '_' };
	const u32 TEXT_COLOR = 0xFFFFFF, BOX_COLOR = 0x343347, OUTLINE_COLOR = 0x13121a;
	
	std::string parse(std::string message)
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

	void advanceChar()
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

	void update()
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

	void render()
	{
		// Draw text box
		Graphics::drawRectangle(BX_PADDING, Graphics::bufferHeight - height - BY_PADDING, width, height, BOX_COLOR);
		Graphics::drawRectangleOutline(BX_PADDING, Graphics::bufferHeight - height - BY_PADDING, width, height, OUTLINE_COLOR);
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
			Graphics::drawText(lines.at(i), BX_PADDING + TX_PADDING, Graphics::bufferHeight - height - BY_PADDING + 
				TY_PADDING + i * (Graphics::CHARACTER_HEIGHT + LINE_PADDING), TEXT_COLOR);
	}

	void setMessage(std::string message)
	{
		Textbox::originalMessage = message; 
		Textbox::buffer = parse(message + PAUSE_CHAR);
		paused = complete = false;
		blinkChar = cUpdates = bUpdates = nChars = 0;
	}

	void setDimensions(int rows, int cols, int charWidth, int charHeight)
	{
		Textbox::width = TX_PADDING * 2 + cols * charWidth;
		Textbox::height = TY_PADDING * 2 + rows * charHeight + (rows - 1) * LINE_PADDING;
		Textbox::cols = cols;
		Textbox::rows = rows;
	}

	void unPause()
	{
		paused = false; 
		textSpeed = TEXT_DEFAULT_SPEED;
	}

	bool isComplete(){ return (mode == TB_STATIC || complete); }
	bool isPaused() { return paused; }
}
