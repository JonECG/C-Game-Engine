#include "FlagBearerStateCamp.h"
#include "../FlagBearer.h"
#include "../Flag.h"

#include "glm\ext.hpp"

int FlagBearerStateCamp::id = 0;

void FlagBearerStateCamp::init( void * context )
{
	FlagBearer * bearer = (FlagBearer*) context;

	bearer->path = bearer->env->level->pathTo( bearer->position, bearer->env->level->getNode(bearer->env->getHomeNode(!bearer->isRed)) );
	progress = 0;	
}

void FlagBearerStateCamp::update( float dt, void * context )
{
	FlagBearer * bearer = (FlagBearer*) context;

	progress += dt*bearer->speed;
	bearer->position = bearer->path->interpolateByDistance(progress);

	static glm::vec3 previodus = glm::vec3();

	glm::vec3 diff = bearer->position-previodus;
	float totLength = glm::length( diff );
	float xzLength = std::sqrt( diff.x*diff.x + diff.z*diff.z );

	bearer->renderable->where = glm::translate( bearer->position ) * glm::rotate( 180 + 180 * std::atan2( diff.x, diff.z ) / PI, 0.0f, 1.0f, 0.0f ) * glm::rotate( 180*std::asin( diff.y/totLength ) / PI, 1.0f, 0.0f, 0.0f ) * glm::scale( glm::vec3(0.4f) );

	/*if( progress > bearer->path->getTotalLength() && bearer->env->getFlag( !bearer->isRed )->carrier == bearer )
	{
		bearer->env->getFlag( !bearer->isRed )->position = bearer->env->level->getNode( bearer->env->getHomeNode( !bearer->isRed ) );
		bearer->env->getFlag( !bearer->isRed )->carrier = nullptr;
	}*/
}

State* FlagBearerStateCamp::copy()
{
	return new FlagBearerStateCamp();	
}

int FlagBearerStateCamp::getId()
{
	if (FlagBearerStateCamp::id == 0 )
		FlagBearerStateCamp::id = State::nextId++;
	return FlagBearerStateCamp::id;
}