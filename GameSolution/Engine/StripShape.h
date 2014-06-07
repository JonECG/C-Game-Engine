#ifndef STRIPSHAPE_H
#define STRIPSHAPE_H

#include "Shape.h"
class ENGINE_SHARED StripShape : public Shape
{

public:
	StripShape( int number, ... );
	~StripShape();
	void draw( Core::Graphics& g, const Vector2& position ) const;
	void draw( Core::Graphics& g, const Matrix4& matrix ) const;
};

#endif