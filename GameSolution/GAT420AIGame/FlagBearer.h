#pragma once
#include "GeneralGLWindow.h"
#include "State.h"
#include "glm\glm.hpp"
#include "WayMapLevel.h"
#include "GameWorld.h"
#include "Decision.h"

class FlagBearer
{
	Decision * decisionTree;
	
	State * currentState;
	
public:
	GeneralGlWindow::Renderable * renderable;
	GameWorld * env;
	glm::vec3 position;

	bool isRed;

	WayPath * path;

	float speed;

	void setState( State * state );

	FlagBearer( GeneralGlWindow::Renderable* renderable, GameWorld * env, Decision * decisionTree, bool isOnRedTeam );
	void paint();
	void update(float dt);
};

