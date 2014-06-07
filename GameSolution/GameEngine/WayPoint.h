#pragma once

#include "glm\glm.hpp"

class WayPoint
{
	int node;
	float heuristic, cost, totalCostEstimate;
	int link;

	bool isOpen;

	WayPoint();
	WayPoint( int node, float heuristic );

	friend class WayMapLevel;
};