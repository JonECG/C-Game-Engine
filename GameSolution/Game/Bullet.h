#ifndef BULLET_H
#define BULLET_H

#include <Engine.h>
#include "Shape.h"

class Bullet
{
	static const float SIZE;
	bool fromPlayer;
	Vector2 velocity, position;
public:
	static Shape *shape;
	Bullet(Vector2 pos, Vector2 vel, bool fromPlayer);
	void update( float dt );
	void draw( Core::Graphics& g, Matrix4& globalTransform ) const;
	Shape* getShape() const;
	Vector2 getPosition() const;
	Vector2 getVelocity() const;
	bool isFromPlayer() const;
};

#endif