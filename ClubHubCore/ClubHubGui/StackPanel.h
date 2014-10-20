#pragma once

#include "UIElement.h"

class EXPORT StackPanel : public UIElement
{
protected:
	glm::vec2 determineAdditionalOffset( int childIndex ) const;

	void updateElement( float deltaTime, const UserController *uc, const glm::vec2& offset );
	void paintElement( Graphics *g, const glm::vec2& offset ) const;
public:
	StackPanel( const glm::vec2& position, const glm::vec2& dimensions );
	~StackPanel();
};

