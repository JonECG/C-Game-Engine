#ifndef LISTSHAPE_H
#define LISTSHAPE_H

#include "Shape.h"

class ENGINE_SHARED ListShape : public Shape
{

public:
	ListShape( int number, ... );
	~ListShape();
	void draw( Core::Graphics& g, const Vector2& position ) const;
	void draw( Core::Graphics& g, const Matrix4& matrix ) const;
};

#endif