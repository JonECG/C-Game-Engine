#pragma once

#include "glmfwd.h"
#include "definition.h"

class CoordinateTransform;

class EXPORT UserController
{
	bool queuedMouse, mouseIsDown, mouseWasDown;
	float *keys;
	bool *wasKeyPressed;
	glm::vec2 *mousePosition;
	CoordinateTransform *coordinateTransform;
public:
	UserController(CoordinateTransform *transform);
	~UserController();

	void update( float dt );
	void queueMouse();

	void setMousePosition( const glm::vec2& position );
	void setMouseDown( bool isDown );

	const glm::vec2& getMousePosition() const;

	bool isMousePressed() const;
	bool isMouseReleased() const;
	bool isMouseDown() const;
	bool isMouseUp() const;

	bool isKeyPressed( int charId ) const;
	bool isKeyReleased( int charId ) const;
	bool isKeyDown( int charId ) const;
	bool isKeyUp( int charId ) const;

	CoordinateTransform *getCoordinateTransform() const;
};

