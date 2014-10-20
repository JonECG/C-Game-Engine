#pragma once

#include "definition.h"
#include "glmfwd.h"

enum CoordinateSystem
{
	OPENGL_COORDINATES,
	SCREEN_COORDINATES,
	PIXELS_COORDINATES
};

class EXPORT CoordinateTransform
{
	CoordinateSystem system;
	glm::vec2 *screen;

	float conversionX( float x, CoordinateSystem from, CoordinateSystem to );
	float conversionY( float y, CoordinateSystem from, CoordinateSystem to );

	float scalingX( float x, CoordinateSystem from, CoordinateSystem to );
	float scalingY( float y, CoordinateSystem from, CoordinateSystem to );
public:
	CoordinateTransform();
	~CoordinateTransform();

	void setCoordinateSystem( CoordinateSystem system );
	void update( float screenWidth, float screenHeight );

	glm::vec2 scale( glm::vec2 vec, CoordinateSystem to );
	float scaleX( float x, CoordinateSystem to );
	float scaleY( float y, CoordinateSystem to );

	glm::vec2 convert( glm::vec2 vec, CoordinateSystem to );
	float convertX( float x, CoordinateSystem to );
	float convertY( float y, CoordinateSystem to );

	glm::vec2 convertFrom( glm::vec2 vec, CoordinateSystem from );
	float convertFromX( float x, CoordinateSystem from );
	float convertFromY( float y, CoordinateSystem from );

	float getYDirection() const;
};
