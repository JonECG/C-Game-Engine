#ifndef SHAPE_H
#define SHAPE_H

#include "Vector2.h"
#include "Matrix.h"
#include <stdarg.h>
#include "Core.h"
#include "Log.h"
#include "ExportHeader.h"

class ENGINE_SHARED Shape
{
protected:
	int count;
	Vector2 * points;

public:
	inline Shape(){};
	Shape( int number, ... );
	~Shape();

	inline void setPoint( int index, Vector2 point );

	inline Vector2 getPoint( int index ) const;
	inline int getCount() const;

	virtual void draw( Core::Graphics& g, const Vector2& position ) const = 0;
	virtual void draw( Core::Graphics& g, const Matrix4& matrix ) const = 0;
	inline void draw( Core::Graphics& g, const Vector2& position, const Core::RGB& color ) const;
	inline void draw( Core::Graphics& g, const Matrix3& matrix, const Core::RGB& color ) const;
	inline void draw( Core::Graphics& g, const Matrix4& matrix, const Core::RGB& color ) const;
	void drawGlow( Core::Graphics& g, const Vector2& position, const Core::RGB& color ) const;
	void drawGlow( Core::Graphics& g, const Matrix4& matrix, const Core::RGB& color ) const;

	inline void scale( float scale );
	inline void flipX();
	inline void flipY();

	bool crossed( const Shape& other, const Vector2& pos1, const Vector2& pos2 ) const;

	Vector2 closestNormal( const Shape& other, const Vector2& position ) const;
	
	Vector2 getCollisionResolve( const Shape& other, const Vector2& position ) const;
};

#include "Shape.inl"

#endif