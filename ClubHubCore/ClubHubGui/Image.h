#pragma once

#include "UIElement.h"
#include "definition.h"

struct Texture;

class EXPORT Image : public UIElement
{
	Texture *texture;
protected:
	void updateElement( float deltaTime, const UserController *uc, const glm::vec2& offset );
	void paintElement( Graphics *g, const glm::vec2& offset ) const;
public:
	Image( const glm::vec2& position, const glm::vec2& dimensions, const Texture& text );
	~Image();
};

