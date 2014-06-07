#pragma once

#include "ForceGenerator.h"
#include "Particle.h"
#include "ContactRegistry.h"
#include "CollisionDetection.h"

class ParticleWorld
{
	int currentParticle;
	Particle* particles[1000];
	
public:
	ContactRegistry contactRegistry;
	ParticleForceRegistry forceRegistry;
	CollisionDetection collisionDetection;

	ParticleWorld();
	void update( float dt );
	void addParticle( Particle * p );
	void clearParticles();
	void addModifier( Particle * p, ParticleForceGenerator * force );
	Particle* operator[]( unsigned int index );
};

