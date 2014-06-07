#ifndef CHASEENEMY_H
#define CHASEENEMY_H

#include "Shape.h"
#include "ListShape.h"
#include "Enemy.h"

class ChaseEnemy : public Enemy
{
public :
	static Shape *shape;
	static const int SPEED = 2000;

	inline void behave( float dt, Ship& ship, std::vector<Bullet>& bul )
	{
		dt; bul;
		Vector2 newAccel = (ship.getPosition()-getPosition()).normalize() * SPEED;
		setAcceleration( newAccel );
	}

	inline Shape* getShape() const
	{
		return shape;
	}
};

#endif