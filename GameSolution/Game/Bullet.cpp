#include "Bullet.h"

//const float Bullet::SIZE = 3;
Shape* Bullet::shape = nullptr; //Shape(19,Vector2(10.8f,0),Vector2(8.2f,-2.5f),Vector2(3.0f,-3.3f),Vector2(-2.8f,-3.1f),Vector2(-4.6f,-2.3f),Vector2(-6.4f,-3.9f),Vector2(-9.2f,-3.7f),Vector2(-7.2f,-2.3f),Vector2(-6.6f,-0.9f),Vector2(-8.0f,-0.7f),Vector2(-8.0f,0.7f),Vector2(-6.6f,0.9f),Vector2(-7.2f,2.3f),Vector2(-9.2f,3.7f),Vector2(-6.4f,3.9f),Vector2(-4.6f,2.3f),Vector2(-2.8f,3.1f),Vector2(3.0f,3.3f),Vector2(8.2f,2.5f));
 
	//Shape( 4, Vector2( -SIZE, SIZE ), Vector2( SIZE, SIZE ), Vector2( SIZE, -SIZE ), Vector2( -SIZE, -SIZE ) );

Bullet::Bullet( Vector2 pos, Vector2 vel, bool fromPlayer )
{
	position = pos;
	velocity = vel;
	this->fromPlayer = fromPlayer;
}

void Bullet::update( float dt )
{
	position += velocity * dt;
}

void Bullet::draw( Core::Graphics& g, Matrix4& globalTransform  ) const
{
	Vector2 nVel = velocity.normalized();
	Matrix4 fun = globalTransform * Matrix4::translation( position ) * Matrix3( nVel, nVel.perpCCW() );
	shape->drawGlow( g, fun, (fromPlayer) ? RGB( 150, 150, 255 ) : RGB( 255, 150, 150 ) );
}

Shape* Bullet::getShape() const
{
	return Bullet::shape;
}

Vector2 Bullet::getPosition() const
{
	return position;
}

Vector2 Bullet::getVelocity() const
{
	return velocity;
}

bool Bullet::isFromPlayer() const
{
	return fromPlayer;
}