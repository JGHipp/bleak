#include "Level.hpp"
#include "../gfx/Camera.hpp"

Level::Level(const char* pathToTilemap)
{
	tilemap = new Tilemap(TILE_SIZE, TILESHEET_TEXTURE, TILE_PATH_METADATA);
	tilemap->loadData(pathToTilemap);
}

Level::~Level()
{
	delete tilemap;
}

void Level::update()
{}

void Level::render(layer_t layer)
{
	if(layer == LEVEL_BACKGROUND)
	{
		tilemap->render(Tilemap::TILE_BACKGROUND);
	}
	else
	{
		tilemap->render(Tilemap::TILE_FOREGROUND);
	}
} 


