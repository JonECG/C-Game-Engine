#include "ForceGenerator.h"

#include "Particle.h"
#include "ContactRegistry.h"

void ParticleForceRegistry::updateForces( float dt )
{
	Registry::iterator i = registrations.begin();
	while( i != registrations.end() )
	{
		i->fg->updateForce( i->particle, dt );
		i++;
	}
}


void ParticleForceRegistry::add( Particle *particle, ParticleForceGenerator *fg )
{
	ParticleForceRegistration reg;
	reg.fg = fg;
	reg.particle = particle;
	registrations.push_back( reg );
}
void ParticleForceRegistry::remove( Particle *particle, ParticleForceGenerator *fg )
{
}
void ParticleForceRegistry::clear()
{
	registrations.clear();
}


ParticleForceGenerator::ParticleForceGenerator()
{
	isActive = true;
}

void ParticleForceGenerator::updateForce( Particle *particle, float dt )
{
	if( isActive )
		updateForceSpecial( particle, dt );
}


ParticleGravity::ParticleGravity( const glm::vec3 &gravity )
{
	this->gravity = gravity;
}

void ParticleGravity::updateForceSpecial( Particle *particle, float dt )
{
	particle->addForce( gravity * particle->mass * dt );
}


ParticleDrag::ParticleDrag( float k1, float k2 )
{
	this->k1 = k1;
	this->k2 = k2;
}

void ParticleDrag::updateForceSpecial( Particle *particle, float dt )
{
	float dragCoeff = glm::length( particle->velocity );
	if( dragCoeff > 0.001f )
	{
		dragCoeff = dt * k1 * dragCoeff + k2 * dragCoeff * dragCoeff;
		// Calculate the final force and apply it.
		glm::vec3 force = glm::normalize( particle->velocity );
		force *= -dragCoeff;
		particle->addForce( force );
	}
}

ParticleSpring::ParticleSpring( Particle* other, float length, float stiffness )
{
	this->other = other;

	this->length = length;
	this->stiffness = stiffness;
}

void ParticleSpring::updateForceSpecial( Particle *particle, float dt )
{
	glm::vec3 springForce = other->position - particle->position;
	springForce = glm::normalize( springForce ) * stiffness * ( glm::length( springForce ) - length );
	particle->addForce( springForce * dt );
}



#include <Windows.h>

ParticleKeyboard::ParticleKeyboard( float speed )
{
	this->speed = speed;
}

void ParticleKeyboard::updateForceSpecial( Particle *particle, float dt )
{
	glm::vec3 direction = glm::vec3( (GetAsyncKeyState(VK_RIGHT)?1:0) - (GetAsyncKeyState(VK_LEFT)?1:0), (GetAsyncKeyState(VK_UP)?1:0) - (GetAsyncKeyState(VK_DOWN)?1:0),0 );
	float length = glm::length( direction );
	if ( length > 0.01 )
		direction /= length;

	particle->addForce( speed * direction * particle->mass * dt );
}