#pragma once

#include "Particle.h"

class ContactRegistry
{
	struct Contact
	{
		Particle *p1, *p2;
		float restitution;
		float penetration;
		glm::vec3 normal;
	} contacts[10000];
	int currentContact;
public:
	ContactRegistry();
	void addContact( Particle *p1, const glm::vec3& normal, float penetration, float restitution = 1 );
	void addContact( Particle *p1, Particle *p2, const glm::vec3& normal, float penetration, float restitution = 1 );
	void resolveContacts( float dt );
};

