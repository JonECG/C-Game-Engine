#pragma once
#include "Component.h"
#include "glm\glm.hpp"
class FollowCamComponent : public Component
{
public:
	Entity * target;
	float laxness;
	glm::vec3 desiredOffset;

	FollowCamComponent();
	void update(float dt);
};

