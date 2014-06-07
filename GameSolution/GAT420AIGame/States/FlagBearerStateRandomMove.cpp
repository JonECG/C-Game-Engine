#include "FlagBearerStateRandomMove.h"
#include "../FlagBearer.h"
#include "../Flag.h"

#include "glm\ext.hpp"

int FlagBearerStateRandomMove::id = 0;

void FlagBearerStateRandomMove::init( void * context )
{
	FlagBearer * bearer = (FlagBearer*) context;

	WayPath * randomPath = bearer->env->level->pathTo( bearer->env->level->getNode( std::rand() % bearer->env->level->getNumberOfNodes() ),  bearer->env->level->getNode( std::rand() % bearer->env->level->getNumberOfNodes() ) );

	bearer->path = bearer->env->level->pathTo( bearer->position, randomPath->interpolateByNode( std::rand() / float(RAND_MAX) ) );
	delete randomPath;
	progress = 0;

	previodus = bearer->position;
}

void FlagBearerStateRandomMove::update( float dt, void * context )
{
	FlagBearer * bearer = (FlagBearer*) context;

	progress += dt*bearer->speed;
	bearer->position = bearer->path->interpolateByDistance(progress);

	glm::vec3 diff = bearer->position-previodus;
	float totLength = glm::length( diff );
	float xzLength = std::sqrt( diff.x*diff.x + diff.z*diff.z );

	bearer->renderable->where = glm::translate( bearer->position ) * glm::rotate( 180 + 180 * std::atan2( diff.x, diff.z ) / PI, 0.0f, 1.0f, 0.0f ) * glm::rotate( 180*std::asin( diff.y/totLength ) / PI, 1.0f, 0.0f, 0.0f ) * glm::scale( glm::vec3(0.4f) );
	
	previodus = bearer->position;

	if( progress > bearer->path->getTotalLength() )
	{
		WayPath * randomPath = bearer->env->level->pathTo( bearer->env->level->getNode( std::rand() % bearer->env->level->getNumberOfNodes() ),  bearer->env->level->getNode( std::rand() % bearer->env->level->getNumberOfNodes() ) );

		bearer->path = bearer->env->level->pathTo( bearer->position, randomPath->interpolateByNode( std::rand() / float(RAND_MAX) ) );
		delete randomPath;
		progress = 0;
	}
}

State* FlagBearerStateRandomMove::copy()
{
	return new FlagBearerStateRandomMove();	
}

int FlagBearerStateRandomMove::getId()
{
	if (FlagBearerStateRandomMove::id == 0 )
		FlagBearerStateRandomMove::id = State::nextId++;
	return FlagBearerStateRandomMove::id;
}