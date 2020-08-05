#include "Level.hpp"

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

void Level::render(Graphics* graphics, Camera* camera, layer_t layer)
{
	if(layer == LEVEL_BACKGROUND)
	{
		tilemap->render(graphics, camera, Tilemap::TILE_BACKGROUND);
	}
	else
	{
		tilemap->render(graphics, camera, Tilemap::TILE_FOREGROUND);
	}
} 


