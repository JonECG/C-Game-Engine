#pragma once

#include "UIElement.h"

class EXPORT Panel : public UIElement
{
	const glm::vec4* color;
protected:
	void updateElement( float deltaTime, const UserController *uc, const glm::vec2& offset );
	void paintElement( Graphics *g, const glm::vec2& offset ) const;
public:
	Panel( const glm::vec2& position, const glm::vec2& dimensions, const glm::vec4& color );
	~Panel();
};

