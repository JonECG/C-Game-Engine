#pragma once

#include "glm\glm.hpp"
#include "GeneralGLWindow.h"
#include "WayMapLevel.h"
#include "GameWorld.h"
#include "FlagBearer.h"

class Flag
{
	
	GeneralGlWindow::Renderable * renderable;
public:
	glm::vec3 position;
	float reset;
	bool isRed;
	FlagBearer * carrier;
	GameWorld * env;

	Flag(GeneralGlWindow::Renderable * renderable, GameWorld * env, bool isRed );
	void update( float dt );
	void paint();
	void goToRandomPosition();
};

