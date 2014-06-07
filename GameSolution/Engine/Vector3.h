#ifndef VECTOR3_H
#define VECTOR3_H

struct Vector3
{
	float x, y, z;

	inline explicit Vector3();
	inline explicit Vector3( float x, float y, float z );
	inline Vector3( const Vector2& v );

	inline float length() const;
	inline float lengthSquared() const;

	inline float dot( const Vector3& other ) const;

	inline operator float*();

	inline friend bool operator==( const Vector3& left, const Vector3& right );
};

#include "Vector3.inl"

#endif