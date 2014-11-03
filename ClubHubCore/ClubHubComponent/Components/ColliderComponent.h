#pragma once
#include "Component.h"
#include "definition.h"
#include "glm\glm.hpp"

class EXPORT ColliderComponent : public Component
{
	int type;
	glm::vec3 bounds;
	float radius;
	glm::vec3 offset;
	friend class Stage;
	bool collidesWith( ColliderComponent * other, glm::vec3 * collisionNormal, float * interpenetration );
public:
	ColliderComponent();
	void setAsBox( glm::vec3 bounds );
	void setAsSphere( float radius );
};

