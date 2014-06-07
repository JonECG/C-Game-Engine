#ifndef TANKENEMY_H
#define TANKENEMY_H

#include "Shape.h"
#include "Enemy.h"

class TankEnemy : public Enemy
{

public :
	static Shape *shape;

	static const int SPEED = 1000;
	static const int BULLET_SPEED = 800;
	static const int TURRET_LENGTH = 30;
	static inline float FIRE_RATE(){ return .9f; }
	float reload;

	inline TankEnemy()
	{
		reload = 0;
		//reload = FIRE_RATE();
	}

	inline void behave( float dt, Ship& ship, std::vector<Bullet>& bul )
	{
		bul;
		reload += dt;
		if ( reload > FIRE_RATE() )
		{
			reload = 0;
			Vector2 direction = acceleration.normalized();
			bul.push_back( Bullet( position + direction * TURRET_LENGTH, direction * BULLET_SPEED + velocity/2, false ) );
		}

		Vector2 newAccel = (ship.getPosition()-getPosition()).normalize() * SPEED;
		setAcceleration( newAccel );
	}

	inline Shape* getShape() const 
	{
		return shape;
	}
};

#endif