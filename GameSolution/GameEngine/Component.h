#pragma once
#include "Engine.h"

class Entity;
class ENGINE_SHARED Component
{	
	int id;
protected:
	Entity * parent;
public:
	Component();
	virtual int getId();
	virtual void init();
	virtual void draw();
	virtual void update( float dt );
	friend Entity;
};

