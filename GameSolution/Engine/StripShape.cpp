#include "StripShape.h"



StripShape::StripShape( int number, ... )
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

StripShape::~StripShape()
{

}

void StripShape::draw( Core::Graphics& g, const Vector2& position ) const
{
	Vector2 hold = (points[count-1] + position);
	for( int i = 0; i < count; i++ )
	{
		Vector2 next = points[i] + position;
		g.DrawLine( hold.x, hold.y, next.x, next.y );
		hold = next;
	}
}

void StripShape::draw( Core::Graphics& g, const Matrix4& matrix ) const
{
	Vector2 hold = matrix * (points[count-1]);
	for( int i = 0; i < count; i++ )
	{
		Vector2 next = matrix * points[i];
		g.DrawLine( hold.x, hold.y, next.x, next.y );
		hold = next;
	}
}