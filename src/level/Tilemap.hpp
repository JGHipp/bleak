#ifndef TILEMAP_H
#define TILEMAP_H

#include "../gfx/Graphics.hpp"
#include "../gfx/Texture.hpp"
#include "Camera.hpp"
#include <vector> // std::vector

class Tilemap
{
	private: 
		Texture* tilemapTexture;
		Texture** tileTextures;
		int* tileData;
		bool tileInRange(int x, int y);
	public: 
		enum solid_t { TILE_PASSABLE, TILE_SOLID }* solidData;
		enum layer_t { TILE_BACKGROUND, TILE_FOREGROUND }* layerData;
		Tilemap(int tileSize, const char* textureFile, const char* metadataFile);
		~Tilemap();
		int width = 0, height = 0, tileSize;
		void init();
		void loadData(const char* pathToLevelFile);
		void loadTileData(const char* pathToTileData);
		void render(Graphics* graphics, Camera* camera, layer_t layer);
		int getTileId(int x, int y);
		void setTile(int x, int y, int id);
		bool isSolid(int id);
		bool isSolid(int x, int y);
		bool isInForeground(int id);
		bool isInForeground(int x, int y);
		Rectangle* getTileRectangle(int x, int y);
		std::vector<Rectangle*> getRectanglesSurrounding(int x, int y); 
};

#endif
