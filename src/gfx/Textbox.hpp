#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "Graphics.hpp"
#include <string>

class Textbox
{
	private:
		std::string buffer;
		int updates = 0, framesPerTick = 0, nChars = 0;
	public:
		const int BOX_HEIGHT = 75, BX_PADDING = 50, BY_PADDING = 10, TX_PADDING = 10, TY_PADDING = 10;
		const int TEXT_COLOR = 0xFFFFFF, BOX_COLOR = 0xFF, OUTLINE_COLOR = 0xFFFFF;
		enum mode_t { TB_STATIC, TB_TYPING } mode;
		Textbox(mode_t mode);
		~Textbox();
		void update();
		void render(Graphics* graphics);
		void setBuffer(std::string buffer);
		void setMode(mode_t mode);
		void setFramesPerTick(int fpt);
		void setNCharsShown(int nChars);
};

#endif
