#ifndef TILEMAP_H
#define TILEMAP_H

#include "../gfx/Graphics.hpp"
#include "../gfx/Texture.hpp"
#include "Camera.hpp"
#include <vector> // std::vector

#define TILE_SOLID 1
#define TILE_PASSABLE 0
#define TILE_BACKGROUND 0
#define TILE_FOREGROUND 1

class Tilemap
{
	private: 
		Texture* tilemapTexture;
		Texture** tileTextures;
		int* tileData;
		bool* solidData, *foregroundData;
		bool tileInRange(int x, int y);
	public: 
		Tilemap(int tileSize, const char* textureFile, const char* metadataFile);
		~Tilemap();
		int width, height, tileSize;
		void init();
		void loadData(const char* pathToLevelFile);
		void loadTileData(const char* pathToTileData);
		void render(Graphics* graphics, Camera* camera, bool layer);
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
