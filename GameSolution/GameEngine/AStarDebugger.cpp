#include "AStarDebugger.h"
#include "DebugShapes.h"
#include "DebugMenus.h"

#ifdef _DEBUG

AStarDebugger::AStarDebugger()
{
	currentWindow = nullptr;
	currentPath = nullptr;
	currentDistance = nullptr;

	railedCamera = false;
	displayPaths = true;
	displayCurrentPath = true;
	displayNodes = true;
}

void AStarDebugger::init( GeneralGlWindow *window, WayMapLevel *level, GeneralGlWindow::ShaderInfo *shader, float &characterSpeed, Camera * camera, WayPath* &currentPath, float &currentDistance )
{
	currentWindow = window;
	currentLevel = level;
	currentShader = shader;
	this->currentPath = &currentPath;
	this->currentDistance = &currentDistance;
	this->camera = camera;
	DebugMenus::toggleBool( "Camera On Rails", railedCamera );
	DebugMenus::toggleBool( "Show Paths", displayPaths );
	DebugMenus::toggleBool( "Show Current Path", displayCurrentPath );
	DebugMenus::toggleBool( "Show Nodes", displayNodes );
	DebugMenus::slideFloat( "Character Speed", characterSpeed, 0.01, 20 ); 
}

void AStarDebugger::paint()
{
	if ( currentLevel != nullptr )
	{
		if( displayNodes )
		{
			for( int i = 0; i < currentLevel->getNumberOfNodes(); i++ )
			{
				glm::vec4 col(1,0,0,1);
			
				DebugShapes::addSphere( currentShader, glm::translate( currentLevel->getNode(i) ) * glm::scale( glm::vec3(0.1) ), glm::vec4( 1,0,0,1 ), 0.00001f, true );
			}
		}

		if( displayPaths )
		{
			for( int i = 0; i < currentLevel->getNumberOfConnections(); i++ )
			{
				DebugShapes::addLine( currentShader, currentLevel->getNode(currentLevel->getConnection(i).from), currentLevel->getNode(currentLevel->getConnection(i).to) + glm::vec3( 0, 0.1, 0 ), glm::vec4( 0,0,1,1), 0.00001f, true );
			}
		}

		if ( currentPath != nullptr )
		{
			if( displayCurrentPath)
			{
				DebugShapes::addSphere( currentShader, glm::translate( (*currentPath)->interpolateByNode((int)((*currentPath)->convertDistanceToNode(*currentDistance)) +1) ) * glm::scale( glm::vec3(0.12) ), glm::vec4( 1,1,0,1 ), 0.00001f, true );
				DebugShapes::addSphere( currentShader, glm::translate( (*currentPath)->interpolateByNode((*currentPath)->getNumberOfNodes()) ) * glm::scale( glm::vec3(0.12) ), glm::vec4( 0,1,0,1 ), 0.00001f, true );
				for( int i = 0; i < (*currentPath)->getNumberOfNodes(); i++ )
				{
					DebugShapes::addLine( currentShader, (*currentPath)->interpolateByNode(i) + glm::vec3( 0, 0.05, 0 ), (*currentPath)->interpolateByNode(i+1) + glm::vec3( 0, 0.05, 0 ), glm::vec4( 1,0,1,1), 0.00001f, true );
				}
			}
			if( railedCamera )
			{
				camera->from = (*currentPath)->interpolateByDistance( *currentDistance - 0.6 ) + glm::vec3( 0, 0.5, 0 );
				camera->to = (*currentPath)->interpolateByDistance( *currentDistance + 0.6 ) + glm::vec3( 0, 0.4, 0 );
			}
		}
	}
}

#endif