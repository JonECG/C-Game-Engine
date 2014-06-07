#ifndef SHIP_H
#define SHIP_H

#include "Engine.h"
#include "Shape.h"
#include "Vector2.h"
#include "graphics.h"
#include "Bullet.h"

class Ship
{
	Vector2 prevPosition;
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;

	Vector2 turret;

	float rotation;

	Matrix2 turretMatrix;

	float reload;

public :
	static Shape *shipShape, *turretShape;

	static inline float FIRE_RATE(){ return 0.3f; }
	static const int SPEED = 2000;
	static const int BULLET_SPEED = 800;
	static const int TURRET_LENGTH = 30;

	Ship();

	inline bool canShoot();
	inline Bullet shoot();

	inline void update( float dt );

	inline void drawTurret( Core::Graphics& g, Matrix4& globalTransform  );

	inline void draw( Core::Graphics& g, Matrix4& globalTransform );

	inline void setVelocity( const Vector2& vec );
	inline void setAcceleration( const Vector2& vec );
	inline void setPosition( const Vector2& vec );

	inline Vector2 getTurret();
	inline void setTurret( const Vector2& vec );

	inline Vector2 getVelocity();
	inline Vector2 getAcceleration();
	inline Vector2 getPosition();
	inline float getRotation();
	inline void setRotation( float rot );
	inline Shape* getShape();
	inline float getX();
	inline float getY();
	inline void goToPrevious();
	
};

#include "Ship.inl"

#endif