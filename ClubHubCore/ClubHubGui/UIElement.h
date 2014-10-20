#pragma once

#include "glmfwd.h"
#include "definition.h"

template<typename T>
class List;

class UserController;
class Graphics;

class EXPORT UIElement
{
	virtual void update( float deltaTime, const UserController *uc, const glm::vec2& offset );
	virtual void paint( Graphics *g, const glm::vec2& offset ) const;

protected:
	glm::vec2 *position, *dimensions;
	List<UIElement*> *children;

	virtual glm::vec2 determineAdditionalOffset( int childIndex ) const;
	UIElement* getChild( int index );

	virtual void updateElement( float deltaTime, const UserController *uc, const glm::vec2& offset ) = 0;
	virtual void paintElement( Graphics *g, const glm::vec2& offset ) const = 0;

public:
	UIElement( const glm::vec2& position, const glm::vec2& dimensions );
	virtual ~UIElement();

	void update( float deltaTime, const UserController *uc );
	void paint( Graphics *g ) const;

	void addChild( UIElement *child );
	void clearChildren();
};