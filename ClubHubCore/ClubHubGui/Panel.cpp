#include "Panel.h"

#include "glm\glm.hpp"
#include "Graphics.h"

Panel::Panel( const glm::vec2& position, const glm::vec2& dimensions, const glm::vec4& color ) : UIElement( position, dimensions )
{
	this->color = new glm::vec4( color );
}

Panel::~Panel()
{
	delete color;
}

void Panel::updateElement( float deltaTime, const UserController *uc, const glm::vec2& offset )
{
	deltaTime;
	uc;
	offset;
}
void Panel::paintElement( Graphics *g, const glm::vec2& offset ) const
{
	glm::vec4 pastColor = g->getColor();
	g->setColor( *color );
	g->fillRect( offset, *dimensions );
	g->setColor( pastColor );
}