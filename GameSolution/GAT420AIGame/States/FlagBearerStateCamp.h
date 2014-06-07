#pragma once
#include "State.h"

class FlagBearerStateCamp : public State
{
	static int id;

	float progress;
public:
	void init( void * context );
	void update( float dt, void * context );
	int getId();
	State* copy();
};
