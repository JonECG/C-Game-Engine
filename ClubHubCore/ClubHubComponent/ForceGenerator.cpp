#include "ForceGenerator.h"
#include "PhysicsComponent.h"

ForceGenerator::ForceGenerator()
{
	isActive = true;
}

void ForceGenerator::updateForce( PhysicsComponent *physics, float dt )
{
	if( isActive )
		updateForceSpecial( physics, dt );
}

GravityForce::GravityForce( const glm::vec3 &gravity )
{
	this->gravity = gravity;
}

void GravityForce::updateForceSpecial( PhysicsComponent *physics, float dt )
{
	physics->addForce( gravity * physics->mass * dt );
}