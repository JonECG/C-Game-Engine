#include "Shape.h"
#include "ColorHelper.h"

Shape::Shape( int number, ... )
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

Shape::~Shape()
{
	delete [] points;
}

void Shape::drawGlow( Core::Graphics& g, const Vector2& position, const Core::RGB& color ) const
{
	const int size = 3;
	const Core::RGB glowColor = RGB( 0,0,0 );
	for( int j = size; j > 0; j-- )
	{
		g.SetColor( ColorHelper::lerp(color,glowColor, j/(float)size) );
		for( int i = 0; i < 4; i++ )
		{
			Vector2 off = ( i/2 == 0 ) ? Vector2( float(i%2)*2 - 1, 0 ) : Vector2( 0, float(i%2)*2 - 1 );
			draw( g, position + off*j );
		}
	}
	g.SetColor( color );
	draw( g, position );
}

void Shape::drawGlow( Core::Graphics& g, const Matrix4& matrix, const Core::RGB& color ) const
{
	const int size = 3;
	const Core::RGB glowColor = RGB( 0,0,0 );
	for( int j = size; j > 0; j-- )
	{
		g.SetColor( ColorHelper::lerp(color,glowColor, j/((float)size) ) );
		for( int i = 0; i < 4; i++ )
		{
			Vector2 off = ( i/2 == 0 ) ? Vector2( float(i%2)*2 - 1, 0 ) : Vector2( 0, float(i%2)*2 - 1 );
			draw( g, Matrix4::translation( off*j ) * matrix );
		}
	}
	g.SetColor( color );
	draw( g, matrix );
}

bool Shape::crossed( const Shape& other, const Vector2& pos1, const Vector2& pos2 ) const
{
	bool result = false;
	for( int j = 0; j < other.count; j++ )
	{
		Vector2 adj1 = other.points[j]+pos1;
		Vector2 adj2 = other.points[j]+pos2; 

		Vector2 hold = (points[count-1]);
		for( int i = 0; i < count; i++ )
		{
			if (!result)
			{
				Vector2 next = points[i];
			
				Vector2 perp = (next-hold).normalize().perpCCW();

				float dist1 = (adj1 - hold).dot(perp);
				float dist2 = (adj2 - hold).dot(perp);

				result = (dist1 > 0) ^ (dist2 > 0);

				hold = next;
			}
		}
	}
	return result;
}

Vector2 Shape::closestNormal( const Shape& other, const Vector2& position ) const
{
	float maxDist = 100000;
	Vector2 result;

	for( int j = 0; j < other.count; j++ )
	{
		Vector2 adj1 = other.points[j]+position;
			
		Vector2 hold = (points[count-1]);
		for( int i = 0; i < count; i++ )
		{
			Vector2 next = points[i];
			
			Vector2 perp = (next-hold).normalize().perpCCW();

			float dist = (adj1 - hold).dot(perp);

			if (std::abs(dist) < std::abs(maxDist))
			{
				maxDist = dist;
				result = perp;
			}

			hold = next;
		}
	}
	return result;
}