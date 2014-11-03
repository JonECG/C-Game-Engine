#pragma once

#include "Window\ManagedAppHandle.h"

class PubHandle : public ManagedAppHandle
{
	void test();
public:
	void init( ContentManager *content, CoordinateTransform *transform, Graphics *g );
	void shutdown();
	void paint( Graphics *g );
	void update( float dt, const UserController *uc );
};

