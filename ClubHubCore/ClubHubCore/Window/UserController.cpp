#include "UserController.h"

#include "CoordinateTransform.h"
#include <Windows.h>
#include <glm\glm.hpp>

const int NUMBER_OF_KEYS = 256;

UserController::UserController(CoordinateTransform *transform)
{
	queuedMouse = false;
	mouseIsDown = false;
	mouseWasDown = false;
	this->coordinateTransform = transform;
	keys = new float[NUMBER_OF_KEYS];
	wasKeyPressed = new bool[NUMBER_OF_KEYS];
	mousePosition = new glm::vec2();
}


UserController::~UserController()
{
	delete keys;
	delete wasKeyPressed;
	delete mousePosition;
}

void UserController::queueMouse()
{
	queuedMouse = true;
}

void UserController::update( float dt )
{
	mouseWasDown = mouseIsDown;

	for( int i = 0; i < NUMBER_OF_KEYS; i++ )
	{
		wasKeyPressed[i] = keys[i] != 0;
		if( GetAsyncKeyState(i) )
		{
			keys[i] += dt;
		}
		else
		{
			keys[i] = 0;
		}
	}
}

const glm::vec2& UserController::getMousePosition() const
{
	return *mousePosition;
}

#include <iostream>
void UserController::setMousePosition( const glm::vec2& position )
{
	*mousePosition = coordinateTransform->convertFrom( position, CoordinateSystem::PIXELS_COORDINATES );
	//std::cout << mousePosition->x << " " << mousePosition->y << std::endl;
}

void UserController::setMouseDown( bool isDown )
{
	mouseIsDown = isDown;
}

bool UserController::isMousePressed() const
{
	return mouseIsDown && !mouseWasDown;
}
bool UserController::isMouseReleased() const
{
	return !mouseIsDown && mouseWasDown;
}
bool UserController::isMouseDown() const
{
	return mouseIsDown;
}
bool UserController::isMouseUp() const
{
	return !mouseIsDown;
}

bool UserController::isKeyPressed( int charId ) const
{
	return isKeyDown( charId ) && !wasKeyPressed[charId];
}
bool UserController::isKeyReleased( int charId ) const
{
	return isKeyUp( charId ) && wasKeyPressed[charId];
}
bool UserController::isKeyDown( int charId ) const
{
	return keys[charId] != 0;
}
bool UserController::isKeyUp( int charId ) const
{
	return keys[charId] == 0;
}

CoordinateTransform* UserController::getCoordinateTransform() const
{
	return coordinateTransform;
}