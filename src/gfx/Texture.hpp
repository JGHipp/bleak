#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL.h>

typedef Uint32 u32;
typedef Uint8 u8;

class Texture
{
	private:
		bool fromSurface = false;
		bool pixelInRange(int x, int y);
		bool pixelInRange(int index);
	public:
		Texture(int width, int height, u32* pixels);
		Texture(SDL_Surface* surface);
		Texture(const char* pathToImage);
		Texture(const char* pathToImage, int xPos, int yPos, int cropWidth, int Height);
		~Texture();
		int width = 0, height = 0;
		u32* pixels;
		Texture* crop(int x, int y, int width, int height);
		u32* getPixels();
		int setPixel(int x, int y, u32 color);
		int setPixel(int index, u32 color);
		u32 getPixel(int x, int y);
		u32 getPixel(int index);
};

#endif
