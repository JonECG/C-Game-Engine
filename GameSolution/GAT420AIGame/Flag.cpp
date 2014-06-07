#include "Flag.h"
#include "glm\ext.hpp"

Flag::Flag(GeneralGlWindow::Renderable * renderable, GameWorld * env, bool isRed )
{
	this->renderable = renderable;
	this->env = env;
	this->isRed = isRed;
	carrier = nullptr;
	goToRandomPosition();
}

void Flag::paint()
{
	renderable->where = glm::translate( position ) * glm::scale( glm::vec3( 0.4 ) );
	renderable->draw();
}

void Flag::update( float dt )
{
	if( carrier != nullptr )
	{
		position = carrier->position;
		reset = 0;
	}
	else
	{
		reset += dt;
		if ( reset > 1.5f )
		{
			goToRandomPosition();
		}
	}
}

void Flag::goToRandomPosition()
{
	WayPath * randomPath = env->level->pathTo( env->level->getNode( std::rand() % env->level->getNumberOfNodes() ),  env->level->getNode( std::rand() % env->level->getNumberOfNodes() ) );
	position = randomPath->interpolateByNode( std::rand() / float(RAND_MAX) );
	reset = 0;
}