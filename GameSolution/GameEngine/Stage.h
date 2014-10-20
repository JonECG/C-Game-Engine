#pragma once
#include "Engine.h"
class Entity;
class CameraComponent;
class GeneralGlWindow;

class ENGINE_SHARED Stage
{
	static const int MAX_ENTITIES = 1000;
	Entity* entities[ MAX_ENTITIES ];
	int currentEntity;

	static const int MAX_RENDERS = 10;
	CameraComponent* renders[ MAX_RENDERS ];
	int currentRender;

	static Stage * stage;
public:
	Stage();
	void addEntity( Entity * entity );
	void update( float dt );
	void subscribeRender( CameraComponent* camera );
	void draw( GeneralGlWindow * window );

	static void setMainStage( Stage * stage );
	static Stage* getMainStage();
};

