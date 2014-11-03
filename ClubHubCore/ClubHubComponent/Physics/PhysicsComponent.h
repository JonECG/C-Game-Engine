#pragma once
#include "Component.h"
#include "definition.h"
#include "glm\glm.hpp"
class ForceGenerator;

class EXPORT PhysicsComponent : public Component
{
	glm::vec3 forceAccum;
	
	int currentForceGenerator;

public:
	glm::vec3 velocity;
	float mass, restitution;
	ForceGenerator ** generators;

	PhysicsComponent();
	~PhysicsComponent();
	void addForce( glm::vec3 force );
	void update( float dt );
	void subscribeForceGenerator( ForceGenerator * fg );
	void onCollide( Entity * other, glm::vec3 collisionNormal, float interpenetration );
};