#pragma once
#include "Component.h"
#include "glm\glm.hpp"
#include "definition.h"

class EXPORT CameraComponent : public Component
{
public:
	float aspect, camNear, camFar, fov;
	CameraComponent();
	void lookAt( glm::vec3 to, glm::vec3 up );
	void update( float dt );
	void draw();
	void sendCamInformation();
};

