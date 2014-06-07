#include "ListShape.h"

ListShape::ListShape( int number, ... )
{
	points = new Vector2[number];
	va_list argPtr;
	va_start( argPtr, number );
	for( int i = 0; i < number; i++ )
	{
		points[i] = va_arg(argPtr,Vector2);
	}
	va_end(argPtr);		

	count = number;
}

ListShape::~ListShape()
{

}

void ListShape::draw( Core::Graphics& g, const Vector2& position ) const
{
	for( int i = 0; i < count-1; i+=2 )
	{
		Vector2 hold = (points[i] + position);
		Vector2 next = points[i+1] + position;
		g.DrawLine( hold.x, hold.y, next.x, next.y );
	}
}

void ListShape::draw( Core::Graphics& g, const Matrix4& matrix ) const
{
	for( int i = 0; i < count-1; i+=2 )
	{
		Vector2 hold = matrix * points[i];
		Vector2 next = matrix * points[i+1];
		g.DrawLine( hold.x, hold.y, next.x, next.y );
	}
}