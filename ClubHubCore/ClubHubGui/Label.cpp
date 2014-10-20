#include "Label.h"

#include "glm\glm.hpp"
#include "Graphics.h"

Label::Label( const glm::vec2& position, const glm::vec2& dimensions, const char* text ) : UIElement( position, dimensions )
{
	setText( text );
}

void Label::updateElement( float deltaTime, const UserController *uc, const glm::vec2& offset )
{
	deltaTime;
	uc;
	offset;
}
void Label::paintElement( Graphics *g, const glm::vec2& offset ) const
{
	glm::vec4 pastColor = g->getColor();
	g->setColor( glm::vec4( 0,0,0,1 ) );
	g->drawText( offset, text );
	g->fillRect( offset, glm::vec2( 0.01, 0.01 ) );
	g->setColor( pastColor );
}

void Label::setText( const char* text )
{
	this->text = text;
}