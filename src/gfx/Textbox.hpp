#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "Graphics.hpp"
#include <string>

#define charToString(x) std::string(1, x) 

namespace Textbox
{
	enum mode_t {TB_STATIC, TB_TYPING };
	extern mode_t mode;
	extern int textSpeed, nChars;
	extern const int TEXT_DEFAULT_SPEED, TEXT_FAST_SPEED;
	extern void setDimensions(int rows, int cols, int charWidth, int charHeight);
	extern void setMessage(std::string message);
	extern void update();
	extern void render();
	extern bool isComplete();
	extern bool isPaused();
	extern void unPause();
};

#endif
