#pragma once
#include "WayPoint.h"
#include "glm\glm.hpp"
#include "Engine.h"

class ENGINE_SHARED WayPath
{
	int numNodes;
	glm::vec3 * nodes;

	float cachedLength;

	WayPath( int numNodes, glm::vec3 * nodes );

public:
	glm::vec3 interpolateByNode( float node );
	glm::vec3 interpolateByDistance( float distance );

	int getNumberOfNodes();
	float getTotalLength();

	float convertDistanceToNode(float distance);

	friend class WayMapLevel;
};