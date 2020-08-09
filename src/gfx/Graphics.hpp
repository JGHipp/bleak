#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "../util/JMath.hpp"
#include "../util/Rectangle.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include <string>
#include <SDL2/SDL.h>

typedef Uint32 u32;

namespace Graphics
{
		extern double windowScale, bufferWidth, bufferHeight;
		extern const int CHARACTER_WIDTH, CHARACTER_HEIGHT;
		extern const u32 TRANSPARENT, CHARACTER_SOLID;
		extern void init(int width, int height, double scale, const char* windowTitle);
		extern void cleanup();
		extern void updateWindow();
		extern void clearBuffer();
		extern int setPixel(int x, int y, u32 color);
		extern void drawRectangle(int xPos, int yPos, int width, int height, u32 color);
		extern void drawRectangle(double xPos, double yPos, int width, int height, u32 color, bool camera);
		extern void drawRectangle(Rectangle rectangle, u32 color);
		extern void drawRectangle(Rectangle rectangle, u32 color, bool camera);
		extern void drawRectangleOutline(int xPos, int yPos, int width, int height, u32 color);
		extern void drawRectangleOutline(double xPos, double yPos, int width, int height, u32 color, bool camera);
		extern void drawRectangleOutline(Rectangle rectangle, u32 color);
		extern void drawRectangleOutline(Rectangle rectangle, u32 color, bool camera);
		extern void drawLine(int x1, int y1, int x2, int y2, u32 color);
		extern void drawLine(double x1, double y1, double x2, double y2, u32 color, bool camera);
		extern void drawVector(int x, int y, double angle, double magnitude, u32 color);
		extern void drawVector(double x, double y, double angle, double magnitude, u32 color, bool camera);
		extern void drawTexture(Texture* texture, int xPos, int yPos);
		extern void drawTexture(Texture* texture, double xPos, double yPos, bool camera);
		extern void drawTextureTransparent(Texture* texture, int xPos, int yPos, u32 transparent);
		extern void drawTextureTransparent(Texture* texture, double xPos, double yPos, u32 transparent, bool camera);
		extern void drawText(std::string text, int xPos, int yPos, u32 color);
		extern void drawText(std::string text, double xPos, double yPos, u32 color, bool camera);
};

#endif
