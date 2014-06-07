#include "Particle.h"

Particle::Particle()
{
	acceleration = glm::vec3();
	velocity = glm::vec3();
	position = glm::vec3();
	prevVelocity = glm::vec3();
	prevPosition = glm::vec3();
	forceAccum = glm::vec3();
}

void Particle::update( float delta )
{
	prevVelocity = velocity;
	prevPosition = position;
	//acceleration += forceAccum / mass;
	velocity += delta * acceleration + forceAccum / mass;
	position += delta * velocity;
	velocity *= damping;
	forceAccum = glm::vec3();
}

void Particle::addForce( const glm::vec3 &force )
{
	forceAccum += force;
}