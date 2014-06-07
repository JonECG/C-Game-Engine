#include "WayMapLevel.h"
#include <fstream>
#include "glm/gtx/transform.hpp"

void WayMapLevel::load( const char* path, GeneralGlWindow * wind, GeneralGlWindow::ShaderInfo * shader, GeneralGlWindow::TextureInfo * tex )
{
	std::ifstream file(path, std::ios::in|std::ios::binary);

	char header[16];

	file.read( header, 16 );

	int vertexCount = *reinterpret_cast<int*>( header );
	int indexCount = *(reinterpret_cast<int*>( header ) + 1);
	int nodeCount = *(reinterpret_cast<int*>( header ) + 2);
	int connectionCount = *(reinterpret_cast<int*>( header ) + 3);


	int vertsSize = (vertexCount) * sizeof(Neumont::Vertex);
	int indexSize = indexCount * sizeof(ushort);

	char* data = new char[ vertsSize + indexSize ];

	file.read( data, vertsSize + indexSize );

	ushort* indices = reinterpret_cast<ushort*>(data);
	Neumont::Vertex* verts = reinterpret_cast<Neumont::Vertex*>(data+indexSize);
	
	GeneralGlWindow::GeometryInfo* levelGeo = wind->addGeometry( verts, vertexCount, indices, indexCount, GL_TRIANGLES );
	wind->setUpAttribs( levelGeo );
	delete [] data;

	int nodeDataSize = sizeof( *nodes ) * nodeCount + sizeof( *connections ) * connectionCount;
	data = new char[ nodeDataSize ];
	file.read( data, nodeDataSize );

	file.close();

	this->numberOfNodes = nodeCount;
	this->nodes = reinterpret_cast<glm::vec3*>(data);
	this->numberOfConnections = connectionCount;
	this->connections = reinterpret_cast<Connection*>(data+sizeof( *nodes ) * nodeCount);
	wayPointList = new WayPoint[ numberOfNodes ];
	levelRenderable = wind->addRenderable( levelGeo, glm::scale( glm::vec3( 0.4f ) ), shader, tex );

	for( int i = 0; i < nodeCount; i++ )
	{
		std::cout << i << " " << nodes[i].x << " " << nodes[i].y << " " << nodes[i].z << std::endl;
	}
}

int WayMapLevel::positionToNodeIndex( const glm::vec3& position )
{
	int node;
	float bestValue = 1000;
	for( int i = 0; i < numberOfNodes; i++ )
	{
		float length = glm::length( nodes[i] - position );
		if (length < bestValue)
		{
			node = i;
			bestValue = length;
		}
	}

	return node;
}

WayPath * WayMapLevel::pathTo( int nodeFrom, int nodeTo )
{
	int numPoints = 0;
	int numOpen = 0;

	bool pathFound = false;

	wayPointList[0] = WayPoint( nodeFrom, glm::length(nodes[nodeTo]-nodes[nodeFrom]) );
	numPoints++;
	numOpen++;

	int end, lastUsed;

	int heldNum = numberOfNodes;
	int heldCon = numberOfConnections;

	while( numOpen > 0 && !pathFound )
	{
		//if( numberOfNodes != heldNum )
		{
			///std::cout << "Crap";
		}
		int bestOpen = -1;
		float bestValue = 1000;
		//Find the best Node
		for( int i = 0; i < numPoints; i++ )
		{
			if( wayPointList[i].totalCostEstimate < bestValue && wayPointList[i].isOpen )
			{
				bestOpen = i;
				bestValue = wayPointList[i].totalCostEstimate;
			}
		}

		//Find all outgoing connections
		for( int i = 0; i < numberOfConnections; i++ )
		{
			if ( connections[i].from == wayPointList[bestOpen].node )
			{
				bool proceed = true;
				int foundIndex = -1;
				for( int j = 0; j < numPoints; j++ )
				{
					if (wayPointList[j].node == connections[i].to)
					{
						foundIndex = j;
						float wouldHaveCost = wayPointList[bestOpen].cost + glm::length(nodes[connections[i].to]-nodes[connections[i].from]);
						if( wayPointList[j].cost < wouldHaveCost )
						{
							proceed = false;
						}
						else
						{
							proceed = true;
						}
					}
				}
				if (proceed)
				{
					if ( foundIndex == -1 )
					{
						foundIndex = numPoints;
						wayPointList[foundIndex] = WayPoint( connections[i].to, glm::length(nodes[nodeTo]-nodes[connections[i].to]) );
						numPoints++;
					}
					wayPointList[foundIndex].isOpen = true;
					wayPointList[foundIndex].link = bestOpen;
					wayPointList[foundIndex].cost = wayPointList[bestOpen].cost + glm::length(nodes[connections[i].to]-nodes[connections[i].from]);
					wayPointList[foundIndex].totalCostEstimate = wayPointList[foundIndex].cost + wayPointList[foundIndex].heuristic;
					lastUsed = wayPointList[foundIndex].node;
					numOpen++;
				}
				if ( connections[i].to == nodeTo )
				{
					pathFound = true;
					end = foundIndex;
				}
			}
		}

		//Add best to closed
		//Todo: adjust pointers
		wayPointList[bestOpen].isOpen = false;
		numOpen--;
		/*for( int i = 0; i < 
		closedList[numClosed] = openList[bestOpen];
		numOpen--;
		openList[bestOpen] = openList[numOpen]; */
	}

	int pathCount = 0;
	int currentNode = 0;
	currentNode = end;
	while( wayPointList[currentNode].node != nodeFrom )
	{
		pathCount++;
		currentNode = wayPointList[currentNode].link;
	}

	glm::vec3 * nodeys = new glm::vec3[ pathCount + 1 ];

	currentNode = end;
	for( int i = 0; i <= pathCount; i++ )
	{
		nodeys[ pathCount-i ] = nodes[wayPointList[currentNode].node];

		std::cout << pathCount-i << " " << wayPointList[currentNode].node << " " << nodeys[pathCount-i].x << " " << nodeys[pathCount-i].y << " " << nodeys[pathCount-i].z << std::endl;

		currentNode = wayPointList[currentNode].link;
	}

	return new WayPath( pathCount + 1, nodeys );
}

WayPath * WayMapLevel::pathTo( const glm::vec3& from, const glm::vec3& to )
{
	WayPath * innerPath = pathTo( positionToNodeIndex( from ), positionToNodeIndex( to ) );

	innerPath->nodes[0] = from;
	innerPath->nodes[innerPath->numNodes-1] = to;

	glm::vec3 * newSet = new glm::vec3[ innerPath->numNodes + 2 ];
	for( int i = 0; i < innerPath->numNodes; i++ )
	{
		newSet[ i + 1 ] = innerPath->nodes[ i ];
	}
	newSet[0] = from;
	newSet[innerPath->numNodes + 1] = to;
	WayPath * fullPath = new WayPath( innerPath->numNodes + 2, newSet );

	//glm::vec3 * newPath = new glm::vec3[ innerPath->getNumberOfNodes() + 1 ];

	//Copy over relevant part of path
	//for( int i = 1; i < innerPath->getNumberOfNodes(); i++ )
	//{
	//
	//}

	return fullPath;
}

int WayMapLevel::getNumberOfNodes()
{
	return numberOfNodes;
}

glm::vec3 WayMapLevel::getNode( int index )
{
	return nodes[index];
}

int WayMapLevel::getNumberOfConnections()
{
	return numberOfConnections;
}

Connection WayMapLevel::getConnection( int index )
{
	return connections[index];
}