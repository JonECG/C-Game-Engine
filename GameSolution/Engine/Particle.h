#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vector2.h"
#include "Core.h"
#include "Matrix4.h"
#include "ExportHeader.h"
#include "ColorHelper.h"
#include "IOrganic.h"

class ENGINE_SHARED Particle : public IOrganic
{
Vector2 position;
Vector2 velocity;
Vector2 acceleration;

Core::RGB color, midColor;
float life, startLife;
int size;
bool fade;

public:
	Particle();
	
	inline void draw( Core::Graphics& g ) const;
	inline void draw( Core::Graphics& g, Matrix4& globalTransform  ) const;
	inline void update( float dt );

	inline void setPosition( const Vector2& vec );
	inline Vector2 getPosition() const;
	
	inline void setVelocity( const Vector2& vec );
	inline Vector2 getVelocity() const;

	inline void setAcceleration( const Vector2& vec );
	inline Vector2 getAcceleration() const;

	inline void setLife( float life );

	inline void setColor( Core::RGB color );
	inline void setColor( Core::RGB color, Core::RGB midColor );

	inline void setSize( int size );

	inline bool isAlive() const;
	inline void kill();

	friend class ParticleEffect;
};

#include "Particle.inl"

#endif