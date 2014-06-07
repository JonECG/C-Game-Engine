#pragma once

#include "ContactRegistry.h"
#include "Particle.h"
#include <vector>

class ParticleCollisionConstraint
{
public:
	bool isActive;
	ContactRegistry *registry;

	ParticleCollisionConstraint(ContactRegistry *registry);
	void checkContraint( Particle *particle );
	virtual void checkConstraintSpecial( Particle *particle ) = 0;
};

class CollisionDetection
{
protected:
	struct ParticleConstraintRegistration
	{
		Particle *particle;
		ParticleCollisionConstraint *fg;
	};

	typedef std::vector<ParticleConstraintRegistration> Registry;
	Registry registrations;

public:
	void addConstraint( Particle *particle, ParticleCollisionConstraint *fg );
	void clearConstraints();

	bool collideWithFloor;
	float restitution;

	CollisionDetection( bool collideWithFloor = true );
	void detectCollisions( Particle ** particles, int numberOfParticles, ContactRegistry * contactRegistry );
};



class ParticleRod : public ParticleCollisionConstraint
{
public:
	Particle *other;
	float length;
	
	ParticleRod( Particle *other, ContactRegistry *contactRegistry, float length );
	virtual void checkConstraintSpecial( Particle *particle );
};