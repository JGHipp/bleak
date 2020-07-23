#include "Texture.hpp"
#include "../util/Log.hpp"
#include <iostream>

Texture::Texture(int width, int height, u32* pixels): width(width), height(height), pixels(pixels)
{}

Texture::Texture(SDL_Surface* surface): fromSurface(true)
{
	width = surface->w;
	height = surface->h;
	pixels = (u32*) surface->pixels;
}

Texture::~Texture()
{
	if(!fromSurface) delete[] pixels;
}

Texture::Texture(const char* pathToImage) 
{ 
	SDL_Surface* surface = SDL_LoadBMP(pathToImage);
	if(surface)
	{
		width = surface->w;
		height = surface->h;
		pixels = new u32[width * height];
		/*
			Must re-map pixels incase they are stored using a different BBP
			If the BBP != 4, then we must re-divide the data with the correct
			bbp, and assign each pixel to a u32. All pixels will be stored in
			a u32 for simplicity. If it is 4, the we have to get rid of the 
			Alpha channel
		*/
		int bbp = surface->format->BytesPerPixel;
		for(int i = 0; i < (width * height); i++) pixels[i] = *(u32*)((u8*) surface->pixels + i * bbp) >> ((bbp - 3) * 8);
	} else error("Problem loading '%s'", pathToImage);
}

Texture::Texture(const char* pathToImage, int xPos, int yPos, int cropWidth, int cropHeight) 
{
	Texture* image = new Texture(pathToImage);
	Texture* croppedImage = image->crop(xPos, yPos, cropWidth, cropHeight);
	pixels = croppedImage->pixels;
	width = croppedImage->width;
	height = croppedImage->height;
}

Texture* Texture::crop(int xPos, int yPos, int cropWidth, int cropHeight)
{
	u32* newPixels = new u32[cropWidth * cropHeight];
	for(int x = 0; x < cropWidth; x++)
	{
		for(int y = 0; y < cropHeight; y++)
			newPixels[x + y * cropWidth] = getPixel(xPos + x, yPos + y);
	}
	return new Texture(cropWidth, cropHeight, newPixels);
}

bool Texture::pixelInRange(int x, int y)
{
	if(x >= 0 && y >= 0 && x < width && y < height) return true;
	return false;
}

bool Texture::pixelInRange(int index)
{
	if(index >= 0 && index < (width * height)) return true;
	return false;
}
	
int Texture::setPixel(int x, int y, u32 color)
{
	if(pixelInRange(x, y))
	{
		pixels[x + y * width] = color & 0xFFFFFF;
		return 0;
	} else return 1;
}

int Texture::setPixel(int index, u32 color)
{
	if(pixelInRange(index))
	{
		pixels[index] = color & 0xFFFFFF;
		return 0;
	} else return 1;
}

u32 Texture::getPixel(int x, int y)
{
	if(pixelInRange(x, y)) return pixels[x + y * width] & 0xFFFFFF;
	return 0;
}
		
u32 Texture::getPixel(int index)
{
	if(pixelInRange(index)) return pixels[index] & 0xFFFFFF;
	return 0;
}

u32* Texture::getPixels() { return pixels; }
