#include "Texture.h"

const int MAX_TEXTURES = 100;
Texture texts[MAX_TEXTURES];
int currentTexture = 0;
Texture* Texture::nextFreeTexture()
{
	return &texts[currentTexture++];
}