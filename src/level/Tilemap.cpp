#include "Tilemap.hpp"
#include "../gfx/Texture.hpp"
#include "../gfx/TextureAtlas.hpp"
#include "../util/JMath.hpp"
#include "../util/Log.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

Tilemap::Tilemap(int tileSize, const char* textureFile, const char* metadataFile): tileSize(tileSize)
{
	tilemapTexture = getTexture(std::string(textureFile));
	loadMetaData(metadataFile);
	init();
}

Tilemap::~Tilemap()
{
	delete tilemapTexture;
	delete[] tileData;
	delete[] solidData;
	delete[] layerData;
	delete[] tileTextures;
}

void Tilemap::init()
{
	// Load texture for each tile
	int nRows = (tilemapTexture->height / tileSize), nCols = (tilemapTexture->width / tileSize);
	for(int id = 0; id < nRows * nCols; id++)
	{
		int tilePosX = id % nCols;
		int tilePosY = id / nCols; 
		Texture* croppedTile = tilemapTexture->crop(tilePosX * tileSize, tilePosY * tileSize, tileSize, tileSize);
		// Initialize the tileTextures array
		if(id == 0) tileTextures = new Texture*[nRows * nCols];
		tileTextures[id] = croppedTile;
	}
}

void Tilemap::loadData(const char* pathToLevelFile)
{
	std::ifstream readFile(pathToLevelFile);
	if(!readFile.is_open()) error("Error reading tile data from '%s'", pathToLevelFile);
	std::vector<std::string> lines;
	std::string line;
	while(std::getline(readFile, line)) lines.push_back(line);
	log("Loading tilemap '%s'", lines.at(0).c_str());
	width = atoi(lines.at(1).c_str());
	height = atoi(lines.at(2).c_str());
	tileData = new int[width * height];
	for(int i = 3; i < (int) lines.size(); i++) tileData[i - 3] = atoi(lines.at(i).c_str());
}

void Tilemap::loadMetaData(const char* pathToMetaData)
{
	std::ifstream readFile(pathToMetaData);
	if(!readFile.is_open()) error("Error reading tile meta data from '%s'", pathToMetaData);
	std::string line;
	std::vector<solid_t>* solid = new std::vector<solid_t>;
	std::vector<layer_t>* foreground = new std::vector<layer_t>;
	while(std::getline(readFile, line))
	{
		std::vector<std::string> tokens;
    	std::stringstream stream(line); 
    	while (stream.good()) 
    	{ 
    		std::string token; 
    	    getline(stream, token, ','); 
    	    tokens.push_back(token); 
    	} 
		solid->push_back(tokens.at(0).compare("true") == 0 ? TILE_SOLID : TILE_PASSABLE);	
		foreground->push_back(tokens.at(1).compare("true") == 0 ? TILE_FOREGROUND : TILE_BACKGROUND);
	} 
	solidData = solid->data();
	layerData = foreground->data();
}

void Tilemap::render(layer_t layer)
{
	// Only render the tiles that are in the view of the camera
	const int padding = 4;
	int nRowTiles = Graphics::bufferWidth / tileSize;
	int nColTiles = Graphics::bufferHeight / tileSize;
	int x1 = clamp((int) Camera::getCenterX() / tileSize - (nRowTiles / 2) - padding, 0, width);
	int x2 = clamp((int) Camera::getCenterX() / tileSize + (nRowTiles / 2) + padding, 0, width);
	int y1 = clamp((int) Camera::getCenterY() / tileSize - (nColTiles / 2) - padding, 0, height);
	int y2 = clamp((int) Camera::getCenterY() / tileSize + (nColTiles / 2) + padding, 0, height);
	for(int x = x1; x <= x2; x++)
	{
		for(int y = y1; y <= y2; y++)
			if((isInForeground(x, y) && layer == TILE_FOREGROUND) || (!isInForeground(x, y) && layer == TILE_BACKGROUND)) 
				Graphics::drawTextureTransparent(tileTextures[getTileId(x, y)], x * tileSize, y * tileSize, Graphics::TRANSPARENT, true);
	}
}

/* 
	Returns Rectangle objects for the tile (x, y)
	lands on, as well as the 8 surrounding tiles
*/
std::vector<Rectangle> Tilemap::getRectanglesSurrounding(int x, int y)
{
	int tx = x / tileSize;
	int ty = y / tileSize;
	std::vector<Rectangle> rectangles;
	#define tryToAdd(a, b) if(isSolid(a, b)) rectangles.push_back(getTileRectangle(a, b));
	tryToAdd(tx, ty);
	tryToAdd(tx - 1, ty - 1); 
	tryToAdd(tx, ty - 1);
	tryToAdd(tx + 1, ty - 1);
	tryToAdd(tx - 1, ty);
	tryToAdd(tx + 1, ty);
	tryToAdd(tx - 1, ty + 1);
	tryToAdd(tx, ty + 1);
	tryToAdd(tx + 1, ty + 1);
	return rectangles;
}

void Tilemap::setTile(int x, int y, int id)
{
	if(tileInRange(x, y) && id >= 0) tileData[x + y * width] = id;
}

int Tilemap::getTileId(int x, int y)
{
	if(tileInRange(x, y)) return tileData[x + y * width];
	return 0;
}

Rectangle Tilemap::getTileRectangle(int x, int y) 
{ return Rectangle(x * tileSize, y * tileSize, tileSize, tileSize); }
bool Tilemap::isSolid(int id) { return (solidData[id] == TILE_SOLID); }
bool Tilemap::isSolid(int x, int y) { return (solidData[getTileId(x, y)] == TILE_SOLID); }
bool Tilemap::isInForeground(int id) { return (layerData[id] == TILE_FOREGROUND); }
bool Tilemap::isInForeground(int x, int y) { return (layerData[getTileId(x, y)] == TILE_FOREGROUND); }
bool Tilemap::tileInRange(int x, int y) { return (x >= 0 && y >= 0 && x < width && y < height); }
