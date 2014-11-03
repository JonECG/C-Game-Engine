#pragma once
#include "glm\glm.hpp"

class PhysicsComponent;

class ForceGenerator
{
public:
	bool isActive;

	ForceGenerator();
	void updateForce( PhysicsComponent *phys, float dt );
	virtual void updateForceSpecial( PhysicsComponent *particle, float duration ) = 0;
};

class GravityForce : public ForceGenerator
{
	glm::vec3 gravity;

public:
	GravityForce( const glm::vec3 &gravity );
	virtual void updateForceSpecial( PhysicsComponent *particle, float dt );
};