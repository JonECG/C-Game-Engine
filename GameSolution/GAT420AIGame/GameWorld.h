#pragma once

#include "WayMapLevel.h"

class FlagBearer;
class Flag;


class GameWorld
{
public:
	GameWorld();
	int redCount;
	int blueCount;

	float redScore;
	float blueScore;

	int redBaseNode;
	int blueBaseNode;

	WayMapLevel * level;

	FlagBearer *redTeam, *blueTeam;
	Flag *redFlag, *blueFlag;

	Flag* getFlag( bool isRed );
	int getHomeNode( bool isRed );
	int getCount( bool isRed );
	void scorePoint( bool isRed );
	FlagBearer* getTeam( bool isRed );

	void update( float dt );
	void paint();
};