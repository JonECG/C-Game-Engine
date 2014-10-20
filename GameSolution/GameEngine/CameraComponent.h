#pragma once
#include "Component.h"
#include "glm\glm.hpp"
class GeneralGlWindow;

class ENGINE_SHARED CameraComponent : public Component
{
public:
	float aspect, camNear, camFar, fov;
	CameraComponent();
	void lookAt( glm::vec3 to, glm::vec3 up );
	void update( float dt );
	void sendCamInformation(GeneralGlWindow * window);
};

