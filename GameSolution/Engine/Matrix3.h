#ifndef MATRIX3_H
#define MATRIX3_H

#include "Matrix2.h"

class Matrix3
{
	float values[9];

public:
	inline Matrix3();
	inline Matrix3( const Vector2& column1, const Vector2& column2 );
	inline Matrix3( const Vector3& column1, const Vector3& column2, const Vector3& column3 );
	inline Matrix3( const Matrix2& mat );

	inline static Matrix3 rotation( float angle );
	inline static Matrix3 scale( float scale );
	inline static Matrix3 scaleX( float scale );
	inline static Matrix3 scaleY( float scale );
	inline static Matrix3 translation( float x, float y );
	inline static Matrix3 translation( const Vector2& offset );

	inline float getCell( int column, int row ) const;
	inline void setCell( int column, int row, float value );

	inline Matrix3 transpose() const;

	inline operator float*();

	inline void operator+=( const Matrix3& other );
	inline void operator-=( const Matrix3& other );
	inline void operator*=( const Matrix3& other );

	inline friend Matrix3 operator+( const Matrix3& left, const Matrix3& right );
	inline friend Matrix3 operator-( const Matrix3& left, const Matrix3& right );
	inline friend Matrix3 operator*( const Matrix3& left, const Matrix3& right );
	inline friend Vector2 operator*( const Matrix3& left, const Vector2& right );
	inline friend Vector3 operator*( const Matrix3& left, const Vector3& right );
};

#include "Matrix3.inl"

#endif