#pragma once

#include "definition.h"

class ManagedAppHandle;

namespace Engine
{
	void EXPORT init( int argc, char* argv[] );
	void EXPORT run( ManagedAppHandle* handle );
	void EXPORT shutdown();
};

