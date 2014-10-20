#include "StackPanel.h"
#include <glm\glm.hpp>

StackPanel::StackPanel( const glm::vec2& position, const glm::vec2& dimensions ) : UIElement( position, dimensions )
{
}
StackPanel::~StackPanel()
{
}

glm::vec2 StackPanel::determineAdditionalOffset( int childIndex ) const
{
	float yOff = 0;
	for( int i = 0; i < childIndex; i++ )
	{
		//yOff += (reinterpret_cast<StackPanel*>(getChild(i)))->dimensions->y;
	}
	return glm::vec2( 0, yOff );
}

void StackPanel::updateElement( float deltaTime, const UserController *uc, const glm::vec2& offset )
{
	deltaTime;
	uc;
	offset;
}
void StackPanel::paintElement( Graphics *g, const glm::vec2& offset ) const
{
	g;
	offset;
}
