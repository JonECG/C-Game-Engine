#pragma once

#include "definition.h"

struct EXPORT Texture
{
	unsigned int textureID;

	//inline Texture( unsigned int texture );
	static Texture* nextFreeTexture();
};

