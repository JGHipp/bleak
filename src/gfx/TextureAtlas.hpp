#ifndef TEXTURE_ATLAS_HPP
#define TEXTURE_ATLAS_HPP

#include "Animation.hpp"
#include <vector>	// std::vector
#include <string>	// std::string

extern std::vector<Animation*> textureAtlasAnimations;
extern std::vector<Texture*> textureAtlasTextures;
extern std::vector<std::string> textureAtlasTextureIds, textureAtlasAnimationIds;

void initTextureAtlas();
void cleanupTextureAtlas();
void addTextureToAtlas(Texture* texture, std::string id);
void addAnimationToAtlas(Animation* animation, std::string id);
#define defineTexture(id, path, x, y, width, height) addTextureToAtlas(new Texture(path, x, y, width, height), id)
void defineAnimation(std::string id, int ticksPerFrame, int nFrames, ...); // Can't do with pre-processor :(
Texture* getTexture(std::string id);
Animation* getAnimation(std::string id);

#endif
