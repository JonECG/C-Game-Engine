#pragma once
#include "OpenGLGameWidget.h"

class EngineTest : public OpenGLGameWidget
{
protected:
	void init();
	void paint();
	void update( float dt );
};

