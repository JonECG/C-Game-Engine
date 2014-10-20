#pragma once

#include "UIElement.h"

class EXPORT Label : public UIElement
{
	const char* text;
protected:
	void updateElement( float deltaTime, const UserController *uc, const glm::vec2& offset );
	void paintElement( Graphics *g, const glm::vec2& offset ) const;
public:
	Label( const glm::vec2& position, const glm::vec2& dimensions, const char* text );
	void setText( const char* text );
};

