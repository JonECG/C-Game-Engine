#include "UIElement.h"
#include "glm\glm.hpp"
#include "List.h"
#include "Graphics.h"
#include "UserController.h"
#include "CoordinateTransform.h"

UIElement::UIElement( const glm::vec2& position, const glm::vec2& dimensions )
{
	this->position = new glm::vec2( position );
	this->dimensions = new glm::vec2( dimensions );
	children = new List<UIElement*>();
}
UIElement::~UIElement()
{
	delete position;
	delete dimensions;
	delete children;
}

glm::vec2 UIElement::determineAdditionalOffset( int childIndex ) const
{
	childIndex;
	return glm::vec2(0,0);
}

UIElement* UIElement::getChild( int index )
{
	return (*children)[index];
}

void UIElement::update( float deltaTime, const UserController *uc, const glm::vec2& offset )
{
	glm::vec2 currentPosition = *position;
	currentPosition.y *= uc->getCoordinateTransform()->getYDirection();

	updateElement( deltaTime, uc, offset + currentPosition );
	for( int i = 0; i < children->getCount(); i++ )
	{
		(*children)[i]->update( deltaTime, uc, currentPosition + offset + determineAdditionalOffset(i) );
	}
}

void UIElement::paint( Graphics *g, const glm::vec2& offset ) const
{
	glm::vec2 currentPosition = *position;
	currentPosition.y *= g->getCoordinateTransform()->getYDirection();

	paintElement( g, offset + currentPosition );
	for( int i = 0; i < children->getCount(); i++ )
	{
		(*children)[i]->paint( g, currentPosition + offset + determineAdditionalOffset(i) );
	}
}

void UIElement::update( float deltaTime, const UserController *uc )
{
	update(deltaTime, uc, glm::vec2(0, position->y - uc->getCoordinateTransform()->getYDirection() * position->y ) );
}

void UIElement::paint( Graphics *g ) const
{
	g->setDepthTesting( false );
	paint(g, glm::vec2(0, position->y - g->getCoordinateTransform()->getYDirection() * position->y ) );
	g->setDepthTesting( true );
}

void UIElement::addChild( UIElement *child )
{
	children->add(child);
}
void UIElement::clearChildren()
{
	children->clear();
}