#include "PhysicsComponent.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "ForceGenerator.h"

const int NUMBER_OF_GENERATORS = 10;

PhysicsComponent::PhysicsComponent()
{
	mass = 1;
	restitution = 0;
	currentForceGenerator = 0;
	generators = reinterpret_cast<ForceGenerator**>( new int[NUMBER_OF_GENERATORS] );
}

PhysicsComponent::~PhysicsComponent()
{
	for( int i = 0; i < currentForceGenerator; i++ )
	{
		delete generators[i];
	}
	delete [] generators;
}

void PhysicsComponent::addForce( glm::vec3 force )
{
	forceAccum += force;
	
}
void PhysicsComponent::update( float dt )
{
	for( int i = 0; i < currentForceGenerator; i++ )
	{
		generators[i]->updateForce( this, dt );
	}
	velocity += forceAccum / mass;
	parent->getComponent<TransformComponent>()->translate( velocity * dt );
	forceAccum = glm::vec3();
}
void PhysicsComponent::subscribeForceGenerator( ForceGenerator * fg )
{
	generators[ currentForceGenerator++ ] = fg;
}

void PhysicsComponent::onCollide( Entity * other, glm::vec3 collisionNormal, float interpenetration )
{
	PhysicsComponent * otherPhys = other->getComponent<PhysicsComponent>();

	/*if( otherPhys == nullptr )
	{
		parent->getComponent<TransformComponent>()->translate( collisionNormal * interpenetration );
	}
	else
	{
		float amount = otherPhys->mass / ( this->mass + otherPhys->mass );
		parent->getComponent<TransformComponent>()->translate( amount * collisionNormal * interpenetration );
	}*/


	float collisionRest = this->restitution;
	if( otherPhys )
		collisionRest *= otherPhys->restitution;

	//resolve velocity
		glm::vec3 relativeVelocity = this->velocity;
		if (otherPhys)
			relativeVelocity -= otherPhys->velocity;
		float separatingVelocity = glm::dot(relativeVelocity, collisionNormal);

		if (separatingVelocity <= 0)
		{
			float newSepVelocity = -separatingVelocity;

			// Check the velocity buildup due to acceleration only.
			glm::vec3 accCausedVelocity = this->forceAccum / this->mass;
			if (otherPhys)
				accCausedVelocity -= otherPhys->forceAccum / otherPhys->mass;
			float accCausedSepVelocity = glm::dot( accCausedVelocity, collisionNormal );

			// If we’ve got a closing velocity due to aceleration buildup, remove it from the new separating velocity.
			if (accCausedSepVelocity < 0)
			{
				newSepVelocity += accCausedSepVelocity / this->restitution;

				// Make sure we haven’t removed more than was there to remove.
				if (newSepVelocity < 0)
					newSepVelocity = 0;
			}
	

			float deltaVelocity = (this->restitution * newSepVelocity - separatingVelocity);

			float totalInverseMass = 1/(this->mass);
			if (otherPhys)
				totalInverseMass += 1/(otherPhys->mass);

			
			float risingAmount = glm::dot( collisionNormal, accCausedVelocity );
			if ( risingAmount > 0 )
				risingAmount = 0;
			glm::vec3 risingVelocity = collisionNormal * risingAmount;
			

			float impulse = deltaVelocity / totalInverseMass;

			glm::vec3 impulsePerIMass = collisionNormal * impulse;

			this->velocity += impulsePerIMass /this->mass;
			this->velocity -= risingVelocity;

			if (otherPhys)
			{
				//otherPhys->velocity -= impulsePerIMass /otherPhys->mass;
				//otherPhys->velocity += risingVelocity;
			}
		}

		//Resolve position
		// If we don’t have any penetration, skip this step. 
		if (interpenetration > 0)
		{
			// The movement of each object is based on their inverse mass so total that. 
			float totalInverseMass = 1/(this->mass);
			if (otherPhys)
				totalInverseMass += 1/(otherPhys->mass);

			// Find the amount of penetration resolution per unit of inverse mass. 
			glm::vec3 movePerIMass = collisionNormal * (interpenetration / totalInverseMass);

			// Apply the penetration resolution.
			parent->getComponent<TransformComponent>()->translate(movePerIMass / this->mass);
			//contacts[i].p1->position += 
			if (otherPhys)
			{
				//contacts[i].p2->position -= movePerIMass / otherPhys->mass;
			}
		}
}