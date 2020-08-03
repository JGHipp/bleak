#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "Graphics.hpp"
#include <string>

#define charToString(x) std::string(1, x) 

class Textbox
{
	private:
		std::string originalMessage, buffer;
		bool paused = false, complete = false;
		int width = 0, height = 0, rows = 0, cols = 0, nChars = 0;
		int cUpdates = 0, bUpdates = 0, blinkChar = 0;
		const int BLINK_SPEED = 10, TICKS_PER_UPDATE = 1;
		const char PAUSE_CHAR = '>', WRAP_CHAR = '\\', CLEAR_CHAR = '^', BLINK[2] = { (char) 219, '_' };
		const u32 TEXT_COLOR = 0xFFFFFF, BOX_COLOR = 0x343347, OUTLINE_COLOR = 0x13121a;
		std::string parse(std::string message);
		void advanceChar();
	public:
		const int BX_PADDING = 50, BY_PADDING = 10, TX_PADDING = 10, TY_PADDING = 10, LINE_PADDING = 5;
		const int TEXT_DEFAULT_SPEED = 2, TEXT_FAST_SPEED = 8;
		enum mode_t { TB_STATIC, TB_TYPING } mode;
		int textSpeed = TEXT_DEFAULT_SPEED;
		Textbox(mode_t mode);
		~Textbox();
		void setDimensions(int rows, int cols, int charWidth, int charHeight);
		void update();
		void render(Graphics* graphics);
		void setMessage(std::string message);
		void setMode(mode_t mode);
		void setSpeed(int textSpeed);
		void setFramesPerTick(int fpt);
		void setNCharsShown(int nChars);
		bool isComplete();
		bool isPaused();
		void unPause();
};

#endif
