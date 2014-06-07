#include "ParticleWorld.h"


ParticleWorld::ParticleWorld()
{
	currentParticle = 0;
}

void ParticleWorld::addParticle( Particle * p )
{
	particles[ currentParticle++ ] = p;
}

void ParticleWorld::clearParticles()
{
	currentParticle = 0;
}

void ParticleWorld::addModifier( Particle * p, ParticleForceGenerator * force )
{
	forceRegistry.add( p, force );
}

Particle* ParticleWorld::operator[]( unsigned int index )
{
	return particles[ index ];
}

void ParticleWorld::update( float dt )
{
	forceRegistry.updateForces( dt );

	for( int inter = 0; inter < 10; inter++ )
	{
		collisionDetection.detectCollisions( particles, currentParticle, &contactRegistry );

		contactRegistry.resolveContacts( dt );
	}

	for( int i = 0; i < currentParticle; i++ )
	{
		particles[i]->update( dt );
	}
}