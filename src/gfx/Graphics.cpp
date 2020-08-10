#include "Graphics.hpp"
#include "TextureAtlas.hpp"
#include <iostream>

namespace Graphics
{
	SDL_Window* window;				// Pointer to window 
	SDL_Surface* windowSurface;		// imageSurface gets drawn to this and scaled
	SDL_Surface* imageSurface;		// What we draw to
	Texture* renderBuffer;			// Texture object representing imageSurface
	Texture** characterBitmap;
	double windowScale, bufferWidth, bufferHeight;
	const int CHARACTER_WIDTH = 8, CHARACTER_HEIGHT = 8;
	const u32 TRANSPARENT = 0xFF00FF, CHARACTER_SOLID = 0xFFFFFF;
	
	void init(int width, int height, double scale, const char* windowTitle)
	{
		windowScale = scale;
		bufferWidth = width;
		bufferHeight = height;
		window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width * scale, height * scale, 0);
		windowSurface = SDL_GetWindowSurface(window);
		imageSurface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0); // 32 bit color, Alpha is unused
   		SDL_SetSurfaceBlendMode(windowSurface, SDL_BLENDMODE_NONE);
		SDL_SetSurfaceBlendMode(imageSurface, SDL_BLENDMODE_NONE);
		renderBuffer = new Texture(imageSurface);
		// Initialize character map
		Texture* fontBitmap = getTexture("font_map");
		std::vector<Texture*>* characters = new std::vector<Texture*>;
		int nCols = (fontBitmap->width / CHARACTER_WIDTH), nRows = (fontBitmap->height / CHARACTER_HEIGHT);
		for(int i = 0; i < nRows * nCols; i++)
		{
			int y = i / nCols;
			int x = i % nCols;
			characters->push_back(fontBitmap->crop(x * CHARACTER_WIDTH, y * CHARACTER_HEIGHT, CHARACTER_WIDTH, CHARACTER_HEIGHT));
		}
		characterBitmap = characters->data();
	}

	void cleanup()
	{
		SDL_FreeSurface(windowSurface);
		SDL_FreeSurface(imageSurface);
		delete renderBuffer;
		SDL_DestroyWindow(window);
	}

	void updateWindow()
	{
		SDL_BlitScaled(imageSurface, NULL, windowSurface, NULL);
		SDL_UpdateWindowSurface(window);
	}

	int setPixel(int x, int y, u32 color)
	{ return renderBuffer->setPixel(x, y, color); }

	void clearBuffer()
	{
		for(int i = 0; i < renderBuffer->width * renderBuffer->height; i++) renderBuffer->setPixel(i, 0);
	}

	void drawRectangle(int xPos, int yPos, int width, int height, u32 color)
	{
		for(int x = 0; x < width; x++)
			for(int y = 0; y < height; y++) setPixel(xPos + x, yPos + y, color);
	}

	void drawRectangleOutline(int xPos, int yPos, int width, int height, u32 color)
	{
		drawLine(xPos, yPos, xPos + width - 1, yPos, color);
		drawLine(xPos, yPos, xPos, yPos + height - 1, color);
		drawLine(xPos + width - 1, yPos, xPos + width - 1, yPos + height - 1, color);
		drawLine(xPos, yPos + height - 1, xPos + width - 1, yPos + height - 1, color);
	}

	void drawRectangle(double xPos, double yPos, int width, int height, u32 color, bool camera)
	{ drawRectangle(camera ? Camera::transformX(xPos) : xPos, camera ? Camera::transformX(yPos) : yPos, width, height, color); }

	void drawRectangle(Rectangle rectangle, u32 color)
	{ drawRectangle(rectangle.x, rectangle.y, rectangle.width, rectangle.height, color); }

	void drawRectangle(Rectangle rectangle, u32 color, bool camera)
	{ drawRectangle(camera ? Camera::transformX(rectangle.x) : rectangle.x, camera ? Camera::transformY(rectangle.y) : rectangle.y, 
		rectangle.width, rectangle.height, color); } 

	void drawRectangleOutline(double xPos, double yPos, int width, int height, u32 color, bool camera)
	{ drawRectangleOutline(camera ? Camera::transformX(xPos) : xPos, camera ? Camera::transformY(yPos) : yPos, width, height, color); }

	void drawRectangleOutline(Rectangle rectangle, u32 color)
	{ drawRectangleOutline(rectangle.x, rectangle.y, rectangle.width, rectangle.height, color); }

	void drawRectangleOutline(Rectangle rectangle, u32 color, bool camera)
	{ drawRectangleOutline(camera ? Camera::transformX(rectangle.x) : rectangle.x, camera ? Camera::transformY(rectangle.y) : rectangle.y,
		rectangle.width, rectangle.height, color); }
	
	/* Algorithm written by John Hippisley
   	Revised version of the Bresenham line drawing 
   	algorithm that supports all directions	
	*/
	void drawLine(int x1, int y1, int x2, int y2, u32 color)
	{
		// Seperate implementation for straight up horizontal/vertical lines
		if(x1 == x2) for(int y = (y1 < y2 ? y1 : y2); y <= (y2 > y1 ? y2 : y1); y++) setPixel(x1, y, color);
		else if(y1 == y2) for(int x = (x1 < x2 ? x1 : x2); x <= (x2 > x1 ? x2 : x1); x++) setPixel(x, y1, color);
		else
		{
			int direction; // 0 = SE, 1 = NE, 2 = SW, 3 = NW
			if(x2 > x1 && y2 > y1) direction = 0;
			else if(x2 > x1 && y2 < y1) direction = 1;
			else if(x1 > x2 && y2 > y1) direction = 2;
			else if(x1 > x2 && y2 < y1) direction = 3;
			else return;
			double dx = abs(x2 - x1);
			double dy = abs(y2 - y1); 
			double step = dx >= dy ? dx : dy;
			if(step == 0.0) return;
			if(direction == 1) dy *= -1;
			else if(direction == 2) dx *= -1;
			else if(direction == 3) 
			{
				dx *= -1;
				dy *= -1;
			} 
			double x = (double) x1;
			double y = (double) y1;
			for(int i = 0; i < step; i++)
			{
				setPixel((int) x, (int) y, color);
				x += dx / step;
				y += dy / step;
			}
		}
	}

	void drawLine(double x1, double y1, double x2, double y2, u32 color, bool camera)
	{ drawLine(camera ? Camera::transformX(x1) : x1, camera ? Camera::transformY(y1) : y1, camera ? Camera::transformX(x2) : x2,
		 camera ? Camera::transformY(y2) : y2, color); }

	void drawVector(int x, int y, double angle, double magnitude, u32 color)
	{ drawLine(x, y, x + dcos(angle) * magnitude, y - dsin(angle) * magnitude, color); }

	void drawVector(double x, double y, double angle, double magnitude, u32 color, bool camera)
	{ drawVector(camera ? Camera::transformX(x) : x, camera ? Camera::transformY(y) : y, angle, magnitude, color); }

	void drawTexture(Texture* texture, int xPos, int yPos)
	{
		for(int x = 0; x < texture->width; x++)
			for(int y = 0; y < texture->height; y++) setPixel(xPos + x, yPos + y, texture->getPixel(x, y));
	}

	void drawTexture(Texture* texture, double xPos, double yPos, bool camera)
	{ drawTexture(texture, camera ? Camera::transformX(xPos) : xPos, camera ? Camera::transformY(yPos) : yPos); }

	void drawTextureTransparent(Texture* texture, int xPos, int yPos, u32 transparent)
	{
		for(int x = 0; x < texture->width; x++)
		{
			for(int y = 0; y < texture->height; y++)
				if(texture->getPixel(x, y) != transparent) setPixel(xPos + x, yPos + y, texture->getPixel(x, y));
		}
	}

	void drawTextureTransparent(Texture* texture, double xPos, double yPos, u32 transparent, bool camera)
	{ drawTextureTransparent(texture, camera ? Camera::transformX(xPos) : xPos, camera ? Camera::transformY(yPos) : yPos, transparent); }

	void drawText(std::string text, int xPos, int yPos, u32 color)
	{
		for(int i = 0; i < (int) text.length(); i++)
		{
			Texture* character = characterBitmap[(int) ((unsigned char) text.at(i))]->crop(0, 0, CHARACTER_WIDTH, CHARACTER_HEIGHT);
			for(int i = 0; i < character->width * character->height; i++)
				if(character->pixels[i] == CHARACTER_SOLID) character->pixels[i] = color;
			drawTextureTransparent(character, xPos + i * CHARACTER_WIDTH, yPos, TRANSPARENT);
		}
	}

	void drawText(std::string text, double xPos, double yPos, u32 color, bool camera)
	{ drawText(text, camera ? Camera::transformX(xPos) : xPos, camera ? Camera::transformY(yPos) : yPos, color); }
}
