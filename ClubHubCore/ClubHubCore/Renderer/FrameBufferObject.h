#pragma once
#include "definition.h"
struct Texture;

struct EXPORT FrameBufferObject
{
	unsigned int frameBufferId;
	Texture *colorTexture, *depthTexture;

	//inline FrameBufferObject(){};
	//inline FrameBufferObject( unsigned int texture, Texture* color, Texture* depth, int width, int height );

	static FrameBufferObject* createFrameBuffer(int width, int height);
	
	int width, height;

	
};

