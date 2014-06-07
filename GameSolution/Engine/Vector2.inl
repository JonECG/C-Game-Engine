#include <math.h>

/*
Vector2
*/

Vector2::Vector2()
{
	x = 0;
	y = 0;
}

Vector2::Vector2( float x, float y )
{
	this->x = x;
	this->y = y;
}

float Vector2::length() const
{
	return sqrt( lengthSquared() );
}
float Vector2::lengthSquared() const
{
	return dot( *this, *this );
}

float Vector2::dot( const Vector2& left, const Vector2& right )
{
	return left.x*right.x + left.y*right.y;
}

float Vector2::cross( const Vector2& left, const Vector2& right )
{
	return left.x*right.x - left.y*right.y;
}

Vector2 Vector2::normalize()
{
	float length = this->length();

	if( length != 0 )
	{
		x/=length;
		y/=length;
	}
	else
	{
		//x = 1;
	}

	return *this;
}
Vector2 Vector2::normalized() const
{
	return Vector2(*this).normalize();
}

Vector2 Vector2::reflect( Vector2 normal )
{
	(*this) -= 2 * normal * dot(normal);
	return *this;
}

Vector2 Vector2::lerp( const Vector2& other, float ratio ) const
{
	return (*this)+(other-(*this))*ratio;
}

float Vector2::project( const Vector2& other ) const
{
	float otherLength = other.length();
	return (otherLength != 0) ? dot(other)/(otherLength) : 0;
}

Vector2 Vector2::perpCW() const
{
	return Vector2( y, -x );
}
Vector2 Vector2::perpCCW() const
{
	return Vector2( -y, x );
}

float Vector2::getAngle() const 
{
	return std::atan2(y, x);
}

float Vector2::dot( const Vector2& other ) const
{
	return x*other.x + y*other.y;
}
float Vector2::cross( const Vector2& other ) const
{
	return x*other.x - y*other.y;
}

Vector2 Vector2::fromAngle( float angle, float length )
{
	return Vector2( std::cos( angle ) * length, std::sin( angle ) * length );
}

Vector2::operator float*()
{
	return &x;
}



void Vector2::operator+=( const Vector2& right )
{
	x+=right.x;
	y+=right.y;
}

void Vector2::operator-=( const Vector2& right )
{
	x-=right.x;
	y-=right.y;
}

void Vector2::operator*=( const float right )
{
	x*=right;
	y*=right;
}

void Vector2::operator/=( const float right )
{
	x/=right;
	y/=right;
}

void Vector2::operator*=( const int right )
{
	x*=right;
	y*=right;
}

void Vector2::operator/=( const int right )
{
	x/=right;
	y/=right;
}

bool operator==( const Vector2& left, const Vector2& right )
{
	return (left.x == right.x && left.y == right.y );
}

Vector2 operator+( const Vector2& left, const Vector2& right )
{
	return Vector2( left.x+right.x, left.y+right.y );
}

Vector2 operator-( const Vector2& left, const Vector2& right )
{
	return Vector2( left.x-right.x, left.y-right.y );
}

Vector2 operator-(const Vector2& in) 
{
	return Vector2(-in.x,-in.y);
}

Vector2 operator*( const Vector2& left, const float right )
{
	return Vector2( left.x*right, left.y*right );
}

Vector2 operator/( const Vector2& left, const float right )
{
	return Vector2( left.x/right, left.y/right );
}

Vector2 operator*( const Vector2& left, const int right )
{
	return Vector2( left.x*right, left.y*right );
}

Vector2 operator/( const Vector2& left, const int right )
{
	return Vector2( left.x/right, left.y/right );
}

Vector2 operator*( const float left, const Vector2& right )
{
	return Vector2( right.x*left, right.y*left );
}

Vector2 operator*( const int left, const Vector2& right )
{
	return Vector2( right.x*left, right.y*left );
}

std::ostream& operator<<( std::ostream& left, const Vector2& right )
{
	left << "{" << right.x << ", " << right.y << "}";
}