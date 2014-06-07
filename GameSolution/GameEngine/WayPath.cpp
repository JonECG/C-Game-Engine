#include "WayPath.h"

WayPath::WayPath(int numNodes, glm::vec3 * nodes )
{
	this->numNodes = numNodes;
	this->nodes = nodes;
	cachedLength = -1;
}

glm::vec3 WayPath::interpolateByNode( float node )
{
	if (node > numNodes - 1)
		node = numNodes - 1;
	int from = (int) node;
	float frac = node - from;
	return nodes[from] + ( nodes[from+1] - nodes[from] ) * frac;
}

glm::vec3 WayPath::interpolateByDistance( float distance )
{
	return interpolateByNode( convertDistanceToNode( distance ) );
}

float WayPath::convertDistanceToNode( float distance )
{
	float currentDistance = 0;

	bool wasFound = false;
	float result;
	for( int i = 0; i < numNodes - 1 && !wasFound; i++ )
	{
		float connectionDistance = glm::length(nodes[i+1]-nodes[i]);
		if( currentDistance + connectionDistance > distance )
		{
			wasFound = true;
			float frac = (distance - currentDistance) / ( connectionDistance );
			result = frac + i;
		}
		else
		{
			currentDistance += connectionDistance;
		}
	}

	if( !wasFound )
		result = getNumberOfNodes();

	return result;
}

int WayPath::getNumberOfNodes()
{
	return numNodes;
}
float WayPath::getTotalLength()
{
	if ( cachedLength < 0 )
	{
		cachedLength = 0;
		for( int i = 0; i < numNodes-1; i++ )
		{
			cachedLength += glm::length(nodes[i+1]-nodes[i]);
		}
	}
	return cachedLength;
}
