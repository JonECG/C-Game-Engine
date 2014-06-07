/*
Vector3
*/

Vector3::Vector3()
{
	x = 0;
	y = 0;
	z = 0;
}
Vector3::Vector3( float x, float y, float z )
{
	this->x = x;
	this->y = y;
	this->z = z;
}
Vector3::Vector3( const Vector2& vec )
{
	this->x = vec.x;
	this->y = vec.y;
	this->z = 0;
}

float Vector3::length() const
{
	return sqrt( lengthSquared() );
}
float Vector3::lengthSquared() const
{
	return dot( *this );
}

float Vector3::dot( const Vector3& other ) const
{
	return x*other.x + y*other.y + z*other.z;
}

Vector3::operator float*()
{
	return &x;
}

bool operator==( const Vector3& left, const Vector3& right )
{
	return (left.x == right.x && left.y == right.y && left.z == right.z );
}