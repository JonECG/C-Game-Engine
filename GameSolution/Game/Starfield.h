#ifndef STARFIELD_H
#define STARFIELD_H

#include "Vector2.h"
#include "Matrix4.h"
#include "Core.h"
#include "Shape.h"
#include "Ship.h"

class Starfield
{
	static const int STARAMOUNT = 200;
	Vector2 starPositions[STARAMOUNT];
	float starSizes[STARAMOUNT];
	float starTimes[STARAMOUNT];
	Core::RGB starColors[STARAMOUNT];
	float starDepth[STARAMOUNT];
public:
	static Shape* star;
	Starfield();
	void update( float dt );
	void draw( Core::Graphics& g, Matrix4 globalTransform, Ship& ship );
};

#endif