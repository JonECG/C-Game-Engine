#ifndef VECTOR2_H
#define VECTOR2_H

#include <iostream>
#include "ExportHeader.h"


struct ENGINE_SHARED Vector2
{
	float x, y;


	//ENGINE_SHARED static const Vector2 X, Y, NX, NY, ZERO;


	inline explicit Vector2();

	inline explicit Vector2( float x, float y );

	inline float length() const;
	inline float lengthSquared() const;

	inline Vector2 normalize();
	inline Vector2 normalized() const;

	inline Vector2 reflect( Vector2 normal );

	inline Vector2 perpCW() const;
	inline Vector2 perpCCW() const;

	inline float getAngle() const;

	inline Vector2 lerp( const Vector2& other, float ratio ) const;
	inline float project( const Vector2& other ) const;

	inline float dot( const Vector2& other ) const;
	inline float cross( const Vector2& other ) const;
	inline static float dot( const Vector2& left, const Vector2& right );
	inline static float cross( const Vector2& left, const Vector2& right );

	inline static Vector2 fromAngle( float angle, float length );
	
	inline operator float*();

	inline void operator+=( const Vector2& right );
	inline void operator-=( const Vector2& right );
	inline void operator*=( const float right );
	inline void operator/=( const float right );
	inline void operator*=( const int right );
	inline void operator/=( const int right );

	inline friend bool operator==( const Vector2& left, const Vector2& right );
	inline friend Vector2 operator+( const Vector2& left, const Vector2& right );
	inline friend Vector2 operator-( const Vector2& left, const Vector2& right );
	inline friend Vector2 operator-( const Vector2& in);
	inline friend Vector2 operator*( const Vector2& left, const float right );
	inline friend Vector2 operator/( const Vector2& left, const float right );
	inline friend Vector2 operator*( const Vector2& left, const int right );
	inline friend Vector2 operator/( const Vector2& left, const int right );
	inline friend Vector2 operator*( const float left, const Vector2& right );
	inline friend Vector2 operator*( const int left, const Vector2& right );
	inline friend std::ostream& operator<<( std::ostream& left, const Vector2& right );
};

#include "Vector2.inl"

#endif
