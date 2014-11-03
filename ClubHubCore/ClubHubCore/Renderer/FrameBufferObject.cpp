#include "FrameBufferObject.h"
#include "Renderer\Texture.h"
#include "GL\glew.h"

#include <iostream>

const int MAX_FBOS = 10000;
FrameBufferObject fbos[MAX_FBOS];
int currentFBO = 0;

FrameBufferObject* FrameBufferObject::createFrameBuffer(int width, int height)
{
	Texture * col = Texture::createTexture( width, height );
	Texture * depth = Texture::createTexture( width, height, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT );
	
	unsigned int frameBufferId;

	//int errorPre = glGetError();

	glActiveTexture( GL_TEXTURE0 );
	//int errorOne = glGetError();
	glGenFramebuffers( 1, &frameBufferId );
	//int errorTwo = glGetError();
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, frameBufferId );
	//int errorThree = glGetError();

	glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, col->textureID, 0 );
	glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth->textureID, 0 );

	//int errorFour = glGetError();

	unsigned int status = glCheckFramebufferStatus( GL_DRAW_FRAMEBUFFER );
	if( status != GL_FRAMEBUFFER_COMPLETE )
	{
		//int error = glGetError();
		std::cout << "NO FRAME BUFFER, BRO" << std::endl;
	}

	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );

	fbos[currentFBO].colorTexture = col;// = FrameBufferObject( frameBufferId, col, depth, width, height );
	fbos[currentFBO].depthTexture = depth;
	fbos[currentFBO].width = width;
	fbos[currentFBO].height = height;
	fbos[currentFBO].frameBufferId = frameBufferId;

	currentFBO++;

	return &fbos[currentFBO - 1];
}