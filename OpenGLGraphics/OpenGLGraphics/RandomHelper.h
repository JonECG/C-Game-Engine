#pragma once
#include <stdlib.h>

namespace RandomHelper
{
	inline float random()
	{
		return ((float)rand()) / RAND_MAX;
	}

	inline float random( float max )
	{
		return random()*max;
	}

	inline int random( int max )
	{
		return (int)( random()*max );
	}
};