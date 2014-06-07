#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <random>
#include "Vector2.h"
#include "Vector3.h"
namespace MathHelper
{
	inline int randomInt( int min, int max )
	{
		int diff = max - min;
		int offset = rand() % diff;
		return min + offset;
	}

	inline int randomInt( int max )
	{
		return rand() % max;
	}

	inline long randomColor()
	{
		return RGB( randomInt(256), randomInt(256), randomInt(256) );
	}

	inline float randomFloat()
	{
		return (float) rand() / RAND_MAX;
	}

	inline float randomSignedFloat()
	{
		return 2 * (rand()-RAND_MAX/2.0f) / RAND_MAX;
	}

	const float TWO_PI = 2*3.1415926f;

	inline Vector2 randomUnitVector()
	{
		float angle = TWO_PI * randomFloat();
		return Vector2( cos(angle), sin(angle) );
	}

	inline Vector3 randomUnit3DVector()
	{
		float angle = TWO_PI * randomFloat();
		float z = randomFloat()*2-1;
		float areaEqual = std::sqrt(1-z*z);
		return Vector3( areaEqual*cos(angle), areaEqual*sin(angle), z );
	}

}
#endif