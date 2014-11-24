#pragma once

#include "definition.h"

struct EXPORT Texture
{
	unsigned int textureID;

	//inline Texture( unsigned int texture );
	static Texture* nextFreeTexture();
	static Texture* createTexture(int width, int height, unsigned int component = 0x1908, unsigned int format = 0x1908, unsigned int type = 0x1401);
	void updateTexture(int width, int height, void * data = 0, unsigned int component = 0x1908, unsigned int format = 0x1908, unsigned int type = 0x1401 );
};

