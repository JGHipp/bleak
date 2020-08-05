#include "TextureAtlas.hpp"
#include "../util/Log.hpp"
#include <iostream>
#include <cstdarg>

std::vector<Animation*> textureAtlasAnimations;
std::vector<Texture*> textureAtlasTextures;
std::vector<std::string> textureAtlasTextureIds, textureAtlasAnimationIds;

void initTextureAtlas()
{
	const int PW = 24, PH = 33, PS = 6; // Player width, height, and animation speed
	defineTexture("font_map", "res/font.bmp");
	defineTexture("tile_sheet", "res/tilesheet.bmp");
	defineTexture("player_left_stand", "res/player.bmp", PW * 7, 0, PW, PH);
	defineTexture("player_left_jump", "res/player.bmp", PW * 3, 0, PW, PH);
	defineTexture("player_left_0", "res/player.bmp", PW * 0, 0, PW, PH);
	defineTexture("player_left_1", "res/player.bmp", PW * 1, 0, PW, PH);
	defineTexture("player_left_2", "res/player.bmp", PW * 2, 0, PW, PH);
	defineTexture("player_left_3", "res/player.bmp", PW * 3, 0, PW, PH);
	defineTexture("player_left_4", "res/player.bmp", PW * 4, 0, PW, PH);
	defineTexture("player_left_5", "res/player.bmp", PW * 5, 0, PW, PH);
	defineAnimation("player_walking_left", PS, 6, "player_left_3", "player_left_4", // Start at middle frame, looks better
		"player_left_5", "player_left_0", "player_left_1", "player_left_2");
	defineTexture("player_right_stand", "res/player.bmp", PW * 7, PH, PW, PH);
	defineTexture("player_right_jump", "res/player.bmp", PW * 3, PH, PW, PH);
	defineTexture("player_right_0", "res/player.bmp", PW * 0, PH, PW, PH);
	defineTexture("player_right_1", "res/player.bmp", PW * 1, PH, PW, PH);
	defineTexture("player_right_2", "res/player.bmp", PW * 2, PH, PW, PH);
	defineTexture("player_right_3", "res/player.bmp", PW * 3, PH, PW, PH);
	defineTexture("player_right_4", "res/player.bmp", PW * 4, PH, PW, PH);
	defineTexture("player_right_5", "res/player.bmp", PW * 5, PH, PW, PH);
	defineAnimation("player_walking_right", PS, 6, "player_right_3", "player_right_4",
		"player_right_5", "player_right_0", "player_right_1", "player_right_2");
}

void cleanupTextureAtlas()
{
	textureAtlasAnimations.clear();
	textureAtlasTextures.clear();
	textureAtlasTextureIds.clear();
	textureAtlasAnimationIds.clear();
}

Texture* getTexture(std::string id)
{
	for(int i = 0; i < (int) textureAtlasTextureIds.size(); i++) 
		if(textureAtlasTextureIds.at(i).compare(id) == 0) return textureAtlasTextures.at(i);
	error("Couldn't retrieve texture with ID '%s'", id);
	return nullptr;
}

Animation* getAnimation(std::string id)
{
	for(int i = 0; i < (int) textureAtlasAnimationIds.size(); i++) 
		if(textureAtlasAnimationIds.at(i).compare(id) == 0) return textureAtlasAnimations.at(i);
	error("Couldn't retrieve animat with ID '%s'", id);
	return nullptr;
}

void addTextureToAtlas(Texture* texture, std::string id)
{
	textureAtlasTextures.push_back(texture);
	textureAtlasTextureIds.push_back(id);
}

void addAnimationToAtlas(Animation* texture, std::string id)
{
	textureAtlasAnimations.push_back(texture);
	textureAtlasAnimationIds.push_back(id);
}

inline void defineAnimation(std::string id, int ticksPerFrame, int nFrames, ...)
{
	va_list args;
	va_start(args, nFrames);
	std::vector<Texture*> frames;
	for(int i = 0; i < nFrames; i++)
	{
		std::string texture = std::string(va_arg(args, const char*));
		frames.push_back(getTexture(texture));
	}
	addAnimationToAtlas(new Animation(frames, ticksPerFrame), id);
}

inline void defineTexture(std::string id, const char* path, int x, int y, int width, int height) 
{ addTextureToAtlas(new Texture(path, x, y, width, height), id); }

inline void defineTexture(std::string id, const char* path) 
{ addTextureToAtlas(new Texture(path), id); }
