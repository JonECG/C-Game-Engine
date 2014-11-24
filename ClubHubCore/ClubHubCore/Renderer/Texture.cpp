#include "Texture.h"
#include "GL\glew.h"

const int MAX_TEXTURES = 100;
Texture texts[MAX_TEXTURES];
int currentTexture = 0;
Texture* Texture::nextFreeTexture()
{
	return &texts[currentTexture++];
}

Texture* Texture::createTexture(int width, int height, GLenum component, GLenum format, GLenum type)
{
	unsigned int texture;
	glGenTextures( 1, &texture );

	Texture * t = nextFreeTexture();
	t->textureID = texture;

	glBindTexture( GL_TEXTURE_2D, texture );
	glTexImage2D(GL_TEXTURE_2D, 0, component, width, height, 0, format, type, 0);
	
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	return t;
}

void Texture::updateTexture( int width, int height, void * data, GLenum component, GLenum format, GLenum type )
{
	glBindTexture( GL_TEXTURE_2D, textureID );
	glTexImage2D(GL_TEXTURE_2D, 0, component, width, height, 0, format, type, data);
	
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}