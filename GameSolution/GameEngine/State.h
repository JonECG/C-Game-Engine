#pragma once
#include "Engine.h"

class State
{
protected:
	static ENGINE_SHARED int nextId;
public:
	virtual void init( void * context ) = 0;
	virtual void update( float dt, void * context ) = 0;
	virtual int getId() = 0;
	virtual State* copy() = 0;
};