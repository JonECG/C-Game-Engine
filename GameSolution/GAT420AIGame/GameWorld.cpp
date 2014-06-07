#include "GameWorld.h"

#include "Flag.h"
#include "FlagBearer.h"

GameWorld::GameWorld()
{
	redScore = 0;
	blueScore = 0;
}

void GameWorld::update( float dt )
{
	redFlag->update(dt);
	blueFlag->update(dt);
	for( int i = 0; i < redCount; i++ )
	{
		redTeam[i].update(dt);
	}
	for( int i = 0; i < blueCount; i++ )
	{
		blueTeam[i].update(dt);
	}
}

void GameWorld::paint()
{
	if ( level->levelRenderable != nullptr )
		level->levelRenderable->draw();
	redFlag->paint();
	blueFlag->paint();
	for( int i = 0; i < redCount; i++ )
	{
		redTeam[i].paint();
	}
	for( int i = 0; i < blueCount; i++ )
	{
		blueTeam[i].paint();
	}
}

Flag* GameWorld::getFlag( bool isRed )
{
	return (isRed) ? redFlag : blueFlag ;
}

int GameWorld::getHomeNode( bool isRed )
{
	return (isRed) ? redBaseNode : blueBaseNode ;
}

int GameWorld::getCount( bool isRed )
{
	return (isRed) ? redCount : blueCount ;
}

void GameWorld::scorePoint( bool isRed )
{
	if( isRed )
		redScore++;
	else
		blueScore++;
}

FlagBearer* GameWorld::getTeam( bool isRed )
{
	return (isRed) ? redTeam : blueTeam ;
}