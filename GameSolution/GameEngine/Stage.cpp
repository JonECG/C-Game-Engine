#include "Stage.h"
#include "Entity.h"
#include "CameraComponent.h"
#include "GL\glew.h"

Stage::Stage()
{
	currentEntity = 0;
	currentRender = 0;
}
Stage * Stage::stage = nullptr;

void Stage::setMainStage( Stage * stage )
{
	Stage::stage = stage;
}

Stage* Stage::getMainStage()
{
	return Stage::stage;
}

void Stage::addEntity( Entity * entity )
{
	entities[currentEntity++] = entity;
}

void Stage::update( float dt )
{
	for( int i = 0; i < currentEntity; i++ )
	{
		entities[i]->update( dt );
	}
}

void Stage::subscribeRender( CameraComponent* camera )
{
	renders[currentRender++] = camera;
}

void Stage::draw( GeneralGlWindow * window )
{
	glClearColor( 0.5, 0.1, 0.1, 0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	for( int j = 0; j < currentRender; j++ )
	{
		renders[j]->sendCamInformation( window );
		for( int i = 0; i < currentEntity; i++ )
		{
			entities[i]->draw();
		}
	}
	currentRender = 0;
}