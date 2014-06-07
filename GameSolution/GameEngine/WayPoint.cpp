#include "WayPoint.h"

WayPoint::WayPoint()
{
}

WayPoint::WayPoint( int node, float heuristic )
{
	this->node = node;
	this->heuristic = heuristic;
	cost = 0;
	isOpen = true;
	totalCostEstimate = heuristic;
}