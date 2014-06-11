#pragma once

#include <vector>
#include "glm\glm.hpp"

class Particle;
class ContactRegistry;

class ParticleForceGenerator
{
public:
	bool isActive;

	ParticleForceGenerator();
	void updateForce( Particle *particle, float dt );
	virtual void updateForceSpecial( Particle *particle, float duration ) = 0;
};

class ParticleForceRegistry
{
protected:
	struct ParticleForceRegistration
	{
		Particle *particle;
		ParticleForceGenerator *fg;
	};

	typedef std::vector<ParticleForceRegistration> Registry;
	Registry registrations;

public:
	void add( Particle *particle, ParticleForceGenerator *fg );
	void remove( Particle *particle, ParticleForceGenerator *fg );
	void clear();
	void updateForces( float duration );
};

class ParticleGravity : public ParticleForceGenerator
{
	glm::vec3 gravity;

public:
	ParticleGravity( const glm::vec3 &gravity );
	virtual void updateForceSpecial( Particle *particle, float dt );
};

class ParticleKeyboard : public ParticleForceGenerator
{
	float speed;
public:
	ParticleKeyboard( float speed );
	virtual void updateForceSpecial( Particle *particle, float dt );
};

class ParticleDrag : public ParticleForceGenerator
{
public:
	float k1, k2;
	
	ParticleDrag( float k1, float k2 );
	virtual void updateForceSpecial( Particle *particle, float dt );
};

class ParticleSpring : public ParticleForceGenerator
{
public:
	Particle *other;
	glm::vec3 base;
	bool usePosition;
	float length;
	float stiffness;
	
	ParticleSpring( Particle *other, float length, float stiffness );
	ParticleSpring( glm::vec3 base, float length, float stiffness );
	virtual void updateForceSpecial( Particle *particle, float dt );
};

