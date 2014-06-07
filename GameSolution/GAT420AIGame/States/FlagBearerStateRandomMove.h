#pragma once
#include "State.h"
#include "glm\glm.hpp"

class FlagBearerStateRandomMove : public State
{
	static int id;

	glm::vec3 previodus;

	float progress;
public:
	void init( void * context );
	void update( float dt, void * context );
	int getId();
	State* copy();
};
