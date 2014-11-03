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

	virtual int getNumberOfRuns();
	virtual void startRun( int index );
	virtual void endRun( int index );
	virtual void setUp();
	virtual void breakDown();
};

