#ifndef MATRIX2_H
#define MATRIX2_H

#include "Vector2.h"
#include "Vector3.h"

class Matrix2
{
	float values[4];

public:
	inline Matrix2();
	inline Matrix2( const Vector2& column1, const Vector2& column2 );

	inline static Matrix2 rotation( float angle );
	inline static Matrix2 scale( float scale );
	inline static Matrix2 scaleX( float scale );
	inline static Matrix2 scaleY( float scale );

	inline float getCell( int column, int row ) const;
	inline void setCell( int column, int row, float value );

	inline Matrix2 transpose() const;

	inline operator float*();

	inline void operator+=( const Matrix2& other );
	inline void operator-=( const Matrix2& other );
	inline void operator*=( const Matrix2& other );

	inline friend Matrix2 operator+( const Matrix2& left, const Matrix2& right );
	inline friend Matrix2 operator-( const Matrix2& left, const Matrix2& right );
	inline friend Matrix2 operator*( const Matrix2& left, const Matrix2& right );
	inline friend Vector2 operator*( const Matrix2& left, const Vector2& right );
};

#include "Matrix2.inl"

#endif