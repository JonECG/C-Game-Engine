#pragma once

#include "ManagedAppHandle.h"

class PubHandle : public ManagedAppHandle
{
	void test();
public:
	void init( ContentManager *content, CoordinateTransform *transform );
	void paint( Graphics *g );
	void update( float dt, const UserController *uc );
};

