#pragma once
#include "Component.h"
#include <glm\glm.hpp>
class Entity;

class AIPilotComponent : public Component
{
	glm::vec3 tweenPos;
	float time;
	bool following;
public:
	Entity * target;
	AIPilotComponent();
	void update( float dt );
};

