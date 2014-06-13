#pragma once

#include "ForceGenerator.h"
#include "Particle.h"
#include "ContactRegistry.h"
#include "CollisionDetection.h"
#include "PhysicsGui\VectorGraphic.h"
#include "PhysicsGui\GuiBase.h"

class ParticleWorld
{
	#define MAXWORLDPARTICLES 50

	int currentParticle;
	Particle* particles[MAXWORLDPARTICLES];
	VectorGraphic* graphics[MAXWORLDPARTICLES];
	
public:
	ContactRegistry contactRegistry;
	ParticleForceRegistry forceRegistry;
	CollisionDetection collisionDetection;

	ParticleWorld();
	void init( GuiBase * gui );
	void update( float dt );
	void sync( GuiBase * gui );
	void removeParticle( int index );
	void addParticle( Particle * p );
	void clearParticles();
	void addModifier( Particle * p, ParticleForceGenerator * force );
	Particle* operator[]( unsigned int index );
	VectorGraphic* getGraphic( int index );
	int getNumberOfParticles();
};

