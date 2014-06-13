#include "ParticleWorld.h"
#include <iostream>

ParticleWorld::ParticleWorld()
{
	currentParticle = 0;
}

void ParticleWorld::init( GuiBase * gui )
{
	for( int i = 0; i < MAXWORLDPARTICLES; i++ )
	{
		graphics[i] = gui->addVectorGraphic();
		graphics[i]->color = glm::vec3( 0.5f, 0.5f, 0.5f );
		graphics[i]->displayStyle = DisplayStyle::DS_POINT;
		gui->sync( graphics[i], glm::vec3( 10000,10000,10000) );
	}
}

void ParticleWorld::addParticle( Particle * p )
{
	particles[ currentParticle++ ] = p;
	std::cout << currentParticle << std::endl;
}

void ParticleWorld::removeParticle( int index )
{
	for( int i = index; i < currentParticle - 1; i++ )
	{
		particles[ i ] = particles[ i + 1 ];
		//graphics[ i ] = graphics[ i + 1 ];
	}

	currentParticle--;
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
	for( int i = 0; i < currentParticle; i++ )
	{
		particles[i]->radius = sqrt( particles[i]->mass ) / 2;
	}

	forceRegistry.updateForces( dt );

	for( int inter = 0; inter < 10; inter++ )
	{
		collisionDetection.detectCollisions( particles, currentParticle, &contactRegistry );

		contactRegistry.resolveContacts( dt );

		for( int i = 0; i < currentParticle; i++ )
		{
			particles[i]->radius = sqrt( particles[i]->mass ) / 2;
			if ( particles[i]->mass < 0.001 )
			{
				removeParticle( i );
				i--;
			}
		}
	}

	for( int i = 0; i < currentParticle; i++ )
	{
		particles[i]->update( dt );
	}
}

VectorGraphic* ParticleWorld::getGraphic( int index )
{
	return graphics[index];
}

int ParticleWorld::getNumberOfParticles()
{
	return currentParticle;
}

void ParticleWorld::sync( GuiBase * gui )
{
	static int lastSynced = currentParticle;

	for( int i = 0; i < currentParticle; i++ )
	{
		graphics[i]->pointSize = particles[i]->radius * 2;
		//graphics[i]->rotation = -angle;
		gui->sync( graphics[i], particles[i]->position );
	}

	for( int i = currentParticle; i < lastSynced; i++ )
	{
		gui->sync( graphics[i], glm::vec3( 10000,10000,10000) );
	}

	lastSynced = currentParticle;
}