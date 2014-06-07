#ifndef MATRIX4_H
#define MATRIX4_H

#include "Vector2.h"
#include "Vector3.h"
#include "Matrix3.h"

class Matrix4
{
	float values[16];

public:
	inline Matrix4();
	//inline Matrix4( const Vector2& column1, const Vector2& column2 );
	//inline Matrix4( const Vector3& column1, const Vector3& column2, const Vector3& column3 );
	inline Matrix4( const Matrix3& mat );
	inline Matrix4( const Matrix2& mat );

	inline static Matrix4 rotation( Vector3 axis, float angle );
	inline static Matrix4 translation( float x, float y );
	inline static Matrix4 translation( const Vector2& offset );
	inline static Matrix4 translation( float x, float y, float z );
	inline static Matrix4 translation( const Vector3& offset );

	inline float getCell( int column, int row ) const;
	inline void setCell( int column, int row, float value );

	inline Matrix4 transpose() const;

	inline operator float*();

	//inline void operator+=( const Matrix3& other );
	//inline void operator-=( const Matrix3& other );
	//inline void operator*=( const Matrix3& other );

	//inline friend Matrix3 operator+( const Matrix3& left, const Matrix3& right );
	//inline friend Matrix3 operator-( const Matrix3& left, const Matrix3& right );
	inline friend Matrix4 operator*( const Matrix4& left, const Matrix4& right );
	inline friend Vector2 operator*( const Matrix4& left, const Vector2& right );
	inline friend Vector3 operator*( const Matrix4& left, const Vector3& right );

	inline Matrix4 inverse();
	inline float calcDeterminant();
};

#include "Matrix4.inl"

#endif