#pragma once
#include "Component.h"
#include "glm\glm.hpp"
class FollowCamComponent : public Component
{
public:
	Entity * target;
	glm::vec3 desiredOffset;

	FollowCamComponent();
	void update(float dt);
};

