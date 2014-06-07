#include "FlagBearerStateChaseEnemy.h"
#include "../FlagBearer.h"
#include "../Flag.h"

#include "glm\ext.hpp"

int FlagBearerStateChaseEnemy::id = 0;

void FlagBearerStateChaseEnemy::init( void * context )
{
	FlagBearer * bearer = (FlagBearer*) context;

	if( bearer->env->getFlag(bearer->isRed)->carrier != nullptr )
		bearer->path = bearer->env->level->pathTo( bearer->position, bearer->env->getFlag(bearer->isRed)->carrier->position );
	else
		bearer->path = bearer->env->level->pathTo( bearer->position, bearer->env->getFlag(bearer->isRed)->position );
	progress = 0;	

	lastFlagPosition = bearer->env->getFlag(bearer->isRed)->position;
	previodus = bearer->position;
}

void FlagBearerStateChaseEnemy::update( float dt, void * context )
{
	FlagBearer * bearer = (FlagBearer*) context;

	if( lastFlagPosition != bearer->env->getFlag(!bearer->isRed)->position )
	{
		bearer->path = bearer->env->level->pathTo( bearer->position, bearer->env->getFlag(bearer->isRed)->position );
		lastFlagPosition = bearer->env->getFlag(bearer->isRed)->position;
		progress = 0;
	}

	progress += dt*bearer->speed;
	bearer->position = bearer->path->interpolateByDistance(progress);
	glm::vec3 diff = bearer->position-previodus;
	float totLength = glm::length( diff );
	float xzLength = std::sqrt( diff.x*diff.x + diff.z*diff.z );

	previodus = bearer->position;

	bearer->renderable->where = glm::translate( bearer->position ) * glm::rotate( 180 + 180 * std::atan2( diff.x, diff.z ) / PI, 0.0f, 1.0f, 0.0f ) * glm::rotate( 180*std::asin( diff.y/totLength ) / PI, 1.0f, 0.0f, 0.0f ) * glm::scale( glm::vec3(0.4f) );

	if( glm::length2( lastFlagPosition - bearer->position ) < 0.2 && bearer->env->getFlag( bearer->isRed )->carrier != nullptr )
	{
		bearer->env->getFlag( bearer->isRed )->goToRandomPosition();
		bearer->env->getFlag( bearer->isRed )->carrier = nullptr;
	}
}

State* FlagBearerStateChaseEnemy::copy()
{
	return new FlagBearerStateChaseEnemy();	
}

int FlagBearerStateChaseEnemy::getId()
{
	if (FlagBearerStateChaseEnemy::id == 0 )
		FlagBearerStateChaseEnemy::id = State::nextId++;
	return FlagBearerStateChaseEnemy::id;
}