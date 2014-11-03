#pragma once
#include "definition.h"
class Entity;
class CameraComponent;
class GeneralGlWindow;

class EXPORT Stage
{
	static const int MAX_ENTITIES = 10000;
	Entity* entities[ MAX_ENTITIES ];
	int currentEntity;

	Entity* queuedRemovals[ MAX_ENTITIES ];
	int currentQueued;

	static const int MAX_RENDERS = 10;
	CameraComponent* renders[ MAX_RENDERS ];
	int currentRender;

	friend class Game;
	Game * game;
public:
	Stage();
	~Stage();
	Game* getGame();
	void addEntity( Entity * entity );
	bool hasEntity( Entity * entity );
	void deleteEntity( Entity * entity );
	void clear();
	void update( float dt );
	void subscribeRender( CameraComponent* camera );
	void draw();
};

