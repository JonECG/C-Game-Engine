#include "ContactRegistry.h"


ContactRegistry::ContactRegistry(void)
{
	currentContact = 0;
}

void ContactRegistry::addContact( Particle *p1, const glm::vec3& normal, float penetration, float restitution )
{
	addContact( p1, 0, normal, penetration, restitution );
}
void ContactRegistry::addContact( Particle *p1, Particle *p2, const glm::vec3& normal, float penetration, float restitution )
{
	contacts[ currentContact ].p1 = p1;
	contacts[ currentContact ].p2 = p2;
	contacts[ currentContact ].normal = normal;
	contacts[ currentContact ].penetration = penetration;
	contacts[ currentContact ].restitution = restitution;
	currentContact++;
}

void ContactRegistry::resolveContacts( float dt )
{
	for( int i = 0; i < currentContact; i++ )
	{
		//resolve velocity
		glm::vec3 relativeVelocity = contacts[i].p1->velocity;
		if (contacts[i].p2)
			relativeVelocity -= contacts[i].p2->velocity;
		float separatingVelocity = glm::dot(relativeVelocity, contacts[i].normal);

		if (separatingVelocity <= 0)
		{
			float newSepVelocity = -separatingVelocity;

			// Check the velocity buildup due to acceleration only.
			glm::vec3 accCausedVelocity = contacts[i].p1->forceAccum / contacts[i].p1->mass;
			if (contacts[i].p2)
				accCausedVelocity -= contacts[i].p2->forceAccum / contacts[i].p2->mass;
			float accCausedSepVelocity = glm::dot( accCausedVelocity, contacts[i].normal );

			// If we’ve got a closing velocity due to aceleration buildup, remove it from the new separating velocity.
			if (accCausedSepVelocity < 0)
			{
				newSepVelocity += accCausedSepVelocity / contacts[i].restitution;

				// Make sure we haven’t removed more than was there to remove.
				if (newSepVelocity < 0)
					newSepVelocity = 0;
			}
	

			float deltaVelocity = (contacts[i].restitution * newSepVelocity - separatingVelocity);

			float totalInverseMass = 1/(contacts[i].p1->mass);
			if (contacts[i].p2)
				totalInverseMass += 1/(contacts[i].p2->mass);

			
			float risingAmount = glm::dot( contacts[i].normal, accCausedVelocity );
			if ( risingAmount > 0 )
				risingAmount = 0;
			glm::vec3 risingVelocity = contacts[i].normal * risingAmount;
			

			float impulse = deltaVelocity / totalInverseMass;

			glm::vec3 impulsePerIMass = contacts[i].normal * impulse;

			contacts[i].p1->velocity += impulsePerIMass /contacts[i].p1->mass;
			contacts[i].p1->velocity -= risingVelocity;

			if (contacts[i].p2)
			{
				contacts[i].p2->velocity -= impulsePerIMass /contacts[i].p2->mass;
				contacts[i].p2->velocity += risingVelocity;
			}
		}

		//Resolve position
		// If we don’t have any penetration, skip this step. 
		if (contacts[i].penetration > 0)
		{
			// The movement of each object is based on their inverse mass so total that. 
			float totalInverseMass = 1/(contacts[i].p1->mass);
			if (contacts[i].p2)
				totalInverseMass += 1/(contacts[i].p2->mass);

			// Find the amount of penetration resolution per unit of inverse mass. 
			glm::vec3 movePerIMass = contacts[i].normal * (contacts[i].penetration / totalInverseMass);

			// Apply the penetration resolution.
			contacts[i].p1->position += movePerIMass / contacts[i].p1->mass;
			if (contacts[i].p2)
			{
				contacts[i].p2->position -= movePerIMass / contacts[i].p2->mass;
			}
		}
	}

	currentContact = 0;
}
