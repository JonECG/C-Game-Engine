#include "FlagBearerStateHome.h"
#include "../FlagBearer.h"
#include "../Flag.h"

#include "glm\ext.hpp"

int FlagBearerStateHome::id = 0;

void FlagBearerStateHome::init( void * context )
{
	FlagBearer * bearer = (FlagBearer*) context;

	bearer->path = bearer->env->level->pathTo( bearer->position, bearer->env->level->getNode(bearer->env->getHomeNode(bearer->isRed)) );
	progress = 0;	

	previodus = bearer->position;
}

void FlagBearerStateHome::update( float dt, void * context )
{
	FlagBearer * bearer = (FlagBearer*) context;

	progress += dt*bearer->speed;
	bearer->position = bearer->path->interpolateByDistance(progress);

	glm::vec3 diff = bearer->position-previodus;
	float totLength = glm::length( diff );
	float xzLength = std::sqrt( diff.x*diff.x + diff.z*diff.z );

	previodus = bearer->position;

	bearer->renderable->where = glm::translate( bearer->position ) * glm::rotate( 180 + 180 * std::atan2( diff.x, diff.z ) / PI, 0.0f, 1.0f, 0.0f ) * glm::rotate( 180*std::asin( diff.y/totLength ) / PI, 1.0f, 0.0f, 0.0f ) * glm::scale( glm::vec3(0.4f) );

	if( progress > bearer->path->getTotalLength() && bearer->env->getFlag( !bearer->isRed )->carrier == bearer )
	{
		bearer->env->getFlag( !bearer->isRed )->goToRandomPosition();
		bearer->env->getFlag( !bearer->isRed )->carrier = nullptr;
		bearer->env->scorePoint( bearer->isRed );
	}
}

State* FlagBearerStateHome::copy()
{
	return new FlagBearerStateHome();	
}

int FlagBearerStateHome::getId()
{
	if (FlagBearerStateHome::id == 0 )
		FlagBearerStateHome::id = State::nextId++;
	return FlagBearerStateHome::id;
}