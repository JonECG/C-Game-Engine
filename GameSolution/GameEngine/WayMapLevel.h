#pragma once
#include "glm\glm.hpp"
#include "Connection.h"
#include "WayPath.h"
#include "WayPoint.h"
#include "GeneralGLWindow.h"
#include "Engine.h"

class ENGINE_SHARED WayMapLevel
{
	int numberOfNodes, numberOfConnections;
	glm::vec3 * nodes;
	Connection * connections;

	WayPoint *wayPointList;
public:

	GeneralGlWindow::Renderable * levelRenderable;

	void load( const char* path, GeneralGlWindow * wind, GeneralGlWindow::ShaderInfo * shader, GeneralGlWindow::TextureInfo * tex );
	
	int positionToNodeIndex( const glm::vec3& position );
	WayPath * pathTo( int nodeFrom, int nodeTo );
	WayPath * pathTo( const glm::vec3& from, const glm::vec3& to );

	int getNumberOfNodes();
	glm::vec3 getNode( int index );

	int getNumberOfConnections();
	Connection getConnection( int index );
};

