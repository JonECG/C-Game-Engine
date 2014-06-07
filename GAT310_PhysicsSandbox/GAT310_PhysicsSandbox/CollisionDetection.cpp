#include "CollisionDetection.h"
#include <iostream>
CollisionDetection::CollisionDetection(bool collideWithFloor)
{
	restitution = 1;
	this->collideWithFloor = collideWithFloor;
}

void CollisionDetection::addConstraint( Particle *particle, ParticleCollisionConstraint *fg )
{
	ParticleConstraintRegistration reg;
	reg.fg = fg;
	reg.particle = particle;
	registrations.push_back( reg );
}
void CollisionDetection::clearConstraints()
{
	registrations.clear();
}

void CollisionDetection::detectCollisions( Particle ** particles, int numberOfParticles, ContactRegistry * contactRegistry )
{
	Registry::iterator i = registrations.begin();
	while( i != registrations.end() )
	{
		i->fg->checkContraint( i->particle );
		i++;
	}

	for( int i = 0; i < numberOfParticles; i++ )
	{
		
		for( int j = i + 1; j < numberOfParticles; j++ )
		{
			glm::vec3 difference = particles[i]->position - particles[j]->position;
			float penetration = ( particles[i]->mass + particles[j]->mass ) / 2 - glm::length( difference );
			if( penetration > 0 )
			{
				//std::cout << "Coll" << std::endl;
				//contactRegistry->addContact( particles[i], particles[j], glm::normalize( difference ), penetration, restitution );
			}
		}

		if( collideWithFloor && particles[i]->position.y <= 0.5f )
		{
			//std::cout << "Coll" << std::endl;
			contactRegistry->addContact( particles[i], glm::vec3( 0, 1, 0 ), 0.5 - particles[i]->position.y, restitution );
		}
	}
}

ParticleCollisionConstraint::ParticleCollisionConstraint(ContactRegistry *registry)
{
	isActive = true;
	this->registry = registry;
}

void ParticleCollisionConstraint::checkContraint( Particle *particle )
{
	if( isActive )
		checkConstraintSpecial( particle );
}

ParticleRod::ParticleRod( Particle* other, ContactRegistry* contactManager, float length ) : ParticleCollisionConstraint( contactManager )
{
	this->other = other;

	this->length = length;
}

void ParticleRod::checkConstraintSpecial( Particle *particle )
{
	/*float lengthDelta = length - glm::length( particle->position - other->position );
	if( lengthDelta < 0 )
		registry->addContact( particle, glm::normalize( particle->position - other->position ), std::abs(lengthDelta), 0 );
	else
		registry->addContact( particle, glm::normalize( other->position - particle->position ), std::abs(lengthDelta), 0 );*/
	float currentLen = glm::length( particle->position - other->position );
	float lengthDelta = length - glm::length( particle->position - other->position );
	float lengthPerIMass = lengthDelta / (1/particle->mass + 1/other->mass);
	glm::vec3 normal = glm::normalize( particle->position - other->position );
	particle->position += normal * lengthPerIMass / particle->mass;
	other->position -= normal * lengthPerIMass / other->mass;

	// Find the length of the rod.
	
	// Check if we’re overextended.
	if (currentLen != length) 
	{ 
		glm::vec3 normal = glm::normalize( other->position - particle->position );
		if (currentLen > length)
		{
			registry->addContact( particle, other, normal, 0, 0 );
			//contact->contactNormal = normal;
			//contact->penetration = currentLen - length;
		}
		else
		{
			registry->addContact( particle, other, -normal, 0, 0 );
			//contact->contactNormal = normal * -1;
			//contact->penetration = length - currentLen;
		}

		/*
		// Otherwise, return the contact.
		contact->particle[0] = particle[0];
		contact->particle[1] = particle[1];
		// Calculate the normal.
		Vector3 normal = particle[1]->getPosition() - particle[0]->getPosition();
		normal.normalize();
		// The contact normal depends on whether we’re extending or compressing.
		if (currentLen > length)
		{
			contact->contactNormal = normal;
			contact->penetration = currentLen - length;
		}
		else
		{
			contact->contactNormal = normal * -1;
			contact->penetration = length - currentLen;
		}
		// Always use zero restitution (no bounciness).
		contact->restitution = 0;
		*/
	}

	/**/
}