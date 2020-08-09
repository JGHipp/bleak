#ifndef LEVEL_H
#define LEVEL_H

#include "Tilemap.hpp"

class Level
{
	private:
		const char* TILE_PATH_METADATA	= "res/tile_mdata.data";
		const char* TILESHEET_TEXTURE	= "tile_sheet";
		const int TILE_SIZE	= 16;
	public:
		enum layer_t { LEVEL_BACKGROUND, LEVEL_FOREGROUND };
		Tilemap* tilemap;
		Level(const char* pathToTilemap);
		~Level();
		void update();
		void render(layer_t layer);
};

#endif
