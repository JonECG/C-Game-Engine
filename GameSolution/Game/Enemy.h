#ifndef ENEMY_H
#define ENEMY_H

#include "Shape.h"
#include "Ship.h"
#include <vector>
#include "IOrganic.h"

class Enemy : public IOrganic
{
protected:
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;

	bool alive;

public :

	inline Enemy()
	{
		alive = false;
		position.x = 0;
		position.y = 0;
	}

	virtual inline ~Enemy(){};

	virtual void behave( float dt, Ship& ship, std::vector<Bullet>& bul ) = 0;
	inline void update( float dt );

	inline void draw( Core::Graphics& g ) const;
	inline void draw( Core::Graphics& g, Matrix4& globalTransform ) const;

	inline void setVelocity( const Vector2& vec );
	inline void setAcceleration( const Vector2& vec );
	inline void setPosition( const Vector2& vec );

	inline Vector2 getVelocity();
	inline Vector2 getAcceleration();
	inline Vector2 getPosition();
	virtual Shape* getShape() const = 0;
	inline float getX();
	inline float getY();

	inline bool isAlive() const;
	inline void kill();

	friend class EnemyBatch;
};

#include "Enemy.inl"

#endif