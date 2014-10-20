#include "Button.h"

#include <glm\glm.hpp>
#include "Graphics.h"
#include <functional>
#include "UserController.h"
#include "glm\glm.hpp"

#include "Label.h"
#include "Panel.h"
#include "CoordinateTransform.h"

Button::Button( const glm::vec2& position, const glm::vec2& dimensions, const char* title ) : UIElement( position, dimensions )
{
	pressEvent = new std::function<void()>();
	addChild( new Panel( glm::vec2(), dimensions, glm::vec4( 0.5, 0.5, 0.5, 1 ) ) );
	
	//addChild( new Panel( dimensions/2.0f, dimensions, glm::vec4( 0.75, 0.75, 0.75, 1 ) ) );
	addChild( new Label( dimensions/2.0f, dimensions, title ) );
	isSubscribed = false;
}

Button::~Button()
{
	delete pressEvent;
}

void Button::subscribeMouseClickEvent( std::function<void()> function )
{
	isSubscribed = true;
	*pressEvent = function;
}
	
void Button::updateElement( float deltaTime, const UserController *uc, const glm::vec2& offset )
{
	deltaTime;

	const glm::vec2 mouse = uc->getMousePosition();
	glm::vec2 effectivePosition;
	effectivePosition = offset;

	if( mouse.x > effectivePosition.x && mouse.x < effectivePosition.x + dimensions->x )
	{
		if ( ( uc->getCoordinateTransform()->getYDirection() > 0 && mouse.y > effectivePosition.y && mouse.y < effectivePosition.y + dimensions->y ) || ( uc->getCoordinateTransform()->getYDirection() < 0 && mouse.y < effectivePosition.y && mouse.y > effectivePosition.y - dimensions->y ) )
		{
			if ( isSubscribed && uc->isMouseReleased() )
			{
				(*pressEvent)();
			}
		}
	}
}
void Button::paintElement( Graphics *g, const glm::vec2& offset ) const
{
	g;
	offset;
}