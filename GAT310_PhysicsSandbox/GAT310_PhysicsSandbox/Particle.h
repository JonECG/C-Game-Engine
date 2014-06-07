#pragma once

#include "glm\glm.hpp"

class Particle
{
public:
	glm::vec3 acceleration, velocity, position, prevPosition, prevVelocity;
	float radius, mass, damping;

	Particle();

	void update( float delta );

	glm::vec3 forceAccum;
	void clearAccumulator();
	void addForce( const glm::vec3 &force );
};