#include "Image.h"

#include "Texture.h"
#include "Graphics.h"
#include <glm\glm.hpp>

Image::Image( const glm::vec2& position, const glm::vec2& dimensions, const Texture& text ) : UIElement( position, dimensions )
{
	texture = new Texture;
	texture->textureID = text.textureID;
}
Image::~Image()
{
	delete texture;
}

void Image::updateElement( float deltaTime, const UserController *uc, const glm::vec2& offset )
{
	deltaTime;
	uc;
	offset;
}
void Image::paintElement( Graphics *g, const glm::vec2& offset ) const
{
	glm::vec4 pastColor = g->getColor();
	g->setColor( glm::vec4(1,1,1,1) );
	g->drawImage( offset, *dimensions, texture );
	g->setColor( pastColor );
}
	
