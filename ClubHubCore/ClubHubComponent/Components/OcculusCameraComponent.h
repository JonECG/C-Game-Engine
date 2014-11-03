#pragma once
#include "CameraComponent.h"

struct ovrHmdDesc_;

class EXPORT OcculusCameraComponent : public CameraComponent
{
	const ovrHmdDesc_ * hmd;
public:
	void init();
	void destroy();
	void update( float dt );

	int getNumberOfRuns();
	void startRun( int index );
	void endRun( int index );
	void setUp();
	void breakDown();
};

