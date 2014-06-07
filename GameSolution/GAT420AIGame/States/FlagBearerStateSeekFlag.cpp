#include "FlagBearerStateSeekFlag.h"
#include "../FlagBearer.h"
#include "../Flag.h"

#include "glm\ext.hpp"

int FlagBearerStateSeekFlag::id = 0;

void FlagBearerStateSeekFlag::init( void * context )
{
	FlagBearer * bearer = (FlagBearer*) context;

	bearer->path = bearer->env->level->pathTo( bearer->position, bearer->env->getFlag(!bearer->isRed)->position );
	progress = 0;		

	previodus = bearer->position;
	lastFlagPosition = bearer->env->getFlag(!bearer->isRed)->position;
}

void FlagBearerStateSeekFlag::update( float dt, void * context )
{
	FlagBearer * bearer = (FlagBearer*) context;

	if( lastFlagPosition != bearer->env->getFlag(!bearer->isRed)->position )
	{
		bearer->path = bearer->env->level->pathTo( bearer->position, bearer->env->getFlag(!bearer->isRed)->position );
		lastFlagPosition = bearer->env->getFlag(!bearer->isRed)->position;
		progress = 0;
	}

	progress += dt*bearer->speed;
	bearer->position = bearer->path->interpolateByDistance(progress);

	glm::vec3 diff = bearer->position-previodus;
	float totLength = glm::length( diff );
	float xzLength = std::sqrt( diff.x*diff.x + diff.z*diff.z );

	bearer->renderable->where = glm::translate( bearer->position ) * glm::rotate( 180 + 180 * std::atan2( diff.x, diff.z ) / PI, 0.0f, 1.0f, 0.0f ) * glm::rotate( 180*std::asin( diff.y/totLength ) / PI, 1.0f, 0.0f, 0.0f ) * glm::scale( glm::vec3(0.4f) );

	previodus = bearer->position;

	if( glm::length2( bearer->position - lastFlagPosition ) < 0.1 && bearer->env->getFlag(!bearer->isRed)->carrier == nullptr )
	{
		bearer->env->getFlag(!bearer->isRed)->carrier = bearer;
	}
	/*if( progress > bearer->path->getTotalLength() && bearer->env->getFlag( !bearer->isRed )->carrier == bearer )
	{
		bearer->env->getFlag( !bearer->isRed )->position = bearer->env->level->getNode( bearer->env->getHomeNode( !bearer->isRed ) );
		bearer->env->getFlag( !bearer->isRed )->carrier = nullptr;
	}*/
}

State* FlagBearerStateSeekFlag::copy()
{
	return new FlagBearerStateSeekFlag();	
}

int FlagBearerStateSeekFlag::getId()
{
	if (FlagBearerStateSeekFlag::id == 0 )
		FlagBearerStateSeekFlag::id = State::nextId++;
	return FlagBearerStateSeekFlag::id;
}