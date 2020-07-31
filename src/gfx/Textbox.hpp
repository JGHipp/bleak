#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "Graphics.hpp"
#include <string>

class Textbox
{
	private:
		std::string buffer;
		int blinkChar = 0, cupdates = 0, bUpdates = 0, nChars = 0;
	public:
		const char BLINK[2] = { (char) 219, '_' };
		const int BLINK_SPEED = 10, TEXT_SPEED = 1;
		const int BOX_HEIGHT = 75, BX_PADDING = 50, BY_PADDING = 10, TX_PADDING = 10, TY_PADDING = 10, LINE_PADDING = 5;
		const u32 TEXT_COLOR = 0xFFFFFF, BOX_COLOR = 0x343347, OUTLINE_COLOR = 0x13121a;
		enum mode_t { TB_STATIC, TB_TYPING } mode;
		Textbox(mode_t mode);
		~Textbox();
		void update();
		void render(Graphics* graphics);
		void setBuffer(std::string buffer);
		void setMode(mode_t mode);
		void setFramesPerTick(int fpt);
		void setNCharsShown(int nChars);
		bool isCaughtUp();
};

#endif
