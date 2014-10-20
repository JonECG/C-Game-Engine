#include "CoordinateTransform.h"

#include "glm\glm.hpp"

CoordinateTransform::CoordinateTransform()
{
	screen = new glm::vec2;
	system = CoordinateSystem::OPENGL_COORDINATES;
}
CoordinateTransform::~CoordinateTransform()
{
	delete screen;
}

void CoordinateTransform::setCoordinateSystem( CoordinateSystem system )
{
	this->system = system;
}
void CoordinateTransform::update( float screenWidth, float screenHeight )
{
	screen->x = screenWidth;
	screen->y = screenHeight;
}

float CoordinateTransform::conversionX( float x, CoordinateSystem from, CoordinateSystem to )
{
	float glX;

	switch( from )
	{
	case CoordinateSystem::SCREEN_COORDINATES:
		glX = x * 2 - 1;
		break;
	case CoordinateSystem::PIXELS_COORDINATES:
		glX = x/screen->x * 2 - 1;
		break;
	default:
		glX = x;
	}

	float result;

	switch( to )
	{
	case CoordinateSystem::SCREEN_COORDINATES:
		result = (glX + 1)/2;
		break;
	case CoordinateSystem::PIXELS_COORDINATES:
		result = screen->x*(glX + 1)/2;
		break;
	default:
		result = glX;
	}

	return result;
}

float CoordinateTransform::conversionY( float y, CoordinateSystem from, CoordinateSystem to )
{
	float glY;

	switch( from )
	{
	case CoordinateSystem::SCREEN_COORDINATES:
		glY = -(y * 2 - 1);
		break;
	case CoordinateSystem::PIXELS_COORDINATES:
		glY = -(y/screen->y * 2 - 1);
		break;
	default:
		glY = y;
	}

	float result;

	switch( to )
	{
	case CoordinateSystem::SCREEN_COORDINATES:
		result = (-glY + 1)/2;
		break;
	case CoordinateSystem::PIXELS_COORDINATES:
		result = screen->y*(-glY + 1)/2;
		break;
	default:
		result = glY;
	}

	return result;
}

float CoordinateTransform::scalingX( float x, CoordinateSystem from, CoordinateSystem to )
{
	float glX;

	switch( from )
	{
	case CoordinateSystem::SCREEN_COORDINATES:
		glX = x * 2;
		break;
	case CoordinateSystem::PIXELS_COORDINATES:
		glX = x/screen->x * 2;
		break;
	default:
		glX = x;
	}

	float result;

	switch( to )
	{
	case CoordinateSystem::SCREEN_COORDINATES:
		result = glX / 2;
		break;
	case CoordinateSystem::PIXELS_COORDINATES:
		result = screen->x * glX / 2;
		break;
	default:
		result = glX;
	}

	return result;
}

float CoordinateTransform::scalingY( float y, CoordinateSystem from, CoordinateSystem to )
{
	float glY;

	switch( from )
	{
	case CoordinateSystem::SCREEN_COORDINATES:
		glY = y * 2;
		break;
	case CoordinateSystem::PIXELS_COORDINATES:
		glY = (y/screen->y * 2);
		break;
	default:
		glY = y;
	}

	float result;

	switch( to )
	{
	case CoordinateSystem::SCREEN_COORDINATES:
		result = glY / 2;
		break;
	case CoordinateSystem::PIXELS_COORDINATES:
		result = screen->y * glY / 2;
		break;
	default:
		result = glY;
	}

	return result;
}

glm::vec2 CoordinateTransform::convert( glm::vec2 vec, CoordinateSystem to )
{
	return glm::vec2( convertX( vec.x, to ), convertY( vec.y, to ) );
}
float CoordinateTransform::convertX( float x, CoordinateSystem to )
{
	return conversionX( x, system, to );
}
float CoordinateTransform::convertY( float y, CoordinateSystem to )
{
	return conversionY( y, system, to );
}

glm::vec2 CoordinateTransform::scale( glm::vec2 vec, CoordinateSystem to )
{
	return glm::vec2( scaleX( vec.x, to ), scaleY( vec.y, to ) );
}
float CoordinateTransform::scaleX( float x, CoordinateSystem to )
{
	return scalingX( x, system, to );
}
float CoordinateTransform::scaleY( float y, CoordinateSystem to )
{
	return scalingY( y, system, to );
}

glm::vec2 CoordinateTransform::convertFrom( glm::vec2 vec, CoordinateSystem from )
{
	return glm::vec2( convertFromX( vec.x, from ), convertFromY( vec.y, from ) );
}
float CoordinateTransform::convertFromX( float x, CoordinateSystem from )
{
	return conversionX( x, from, system );
}
float CoordinateTransform::convertFromY( float y, CoordinateSystem from )
{
	return conversionY( y, from, system );
}

float CoordinateTransform::getYDirection() const
{
	float result = 1;

	switch( system )
	{
	case CoordinateSystem::OPENGL_COORDINATES:
		result = -1;
		break;
	default:
		result = 1;
	}

	return result;
}