#include "PubHandle.h"

#include "Shader.h"
#include "ContentManager.h"
#include "Graphics.h"
#include "Font.h"
#include "Character.h"

#include "UserController.h"
#include "CoordinateTransform.h"

#include "glm\glm.hpp"
#include "ShapeGenerator.h"

#include "Game.h"
#include "Stage.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "RenderComponent.h"
#include "ColliderComponent.h"
#include "InputMoveComponent.h"
#include "FollowCamComponent.h"
#include "PhysicsComponent.h"
#include "ForceGenerator.h"
#include "PlaneComponent.h"

Texture *cow;
Font *font;
Game * game;

glm::vec3 lightPos = glm::vec3( 1, 2, 10 );
glm::vec4 difLight = glm::vec4(1.0f,1.0f,1.0f,1), colInfluence = glm::vec4(0.8f,0.8f,0.8f,1), specColor = glm::vec4(0.8f,0.8f,0.8f,1);
float specAmount = 10;

Entity * makeAThing()
{
	Entity * ent = new Entity();
	ent->addComponent( new TransformComponent() );
	ent->addComponent( new RenderComponent() );
	ent->addComponent( new ColliderComponent() );
	ent->getComponent<ColliderComponent>()->setAsBox( glm::vec3( 1, 1, 1 ) );
	//ent->addComponent( new PhysicsComponent() );
	return ent;
}

void PubHandle::init( ContentManager* content, CoordinateTransform *transform )
{
	transform->setCoordinateSystem( CoordinateSystem::OPENGL_COORDINATES );

	cow = content->loadTexture( "Resources/cow.png" );
	font = content->loadFont( "Resources/Tahoma.bfnt" );
	Shader * passShad = content->loadShader( "Resources/passThrough.vert", "Resources/passThrough.frag" );

	game = new Game();
	game->subscribeContentManager( content );

	Stage * stage = new Stage();
	game->setStage( stage );

	Geometry * planeGeo = content->loadGeometry( "Resources/biplane.mod" );
	Geometry * cubeGeo = ShapeGenerator::createCube( content );

	Entity * cam = new Entity();
	cam->addComponent( new TransformComponent() );
	cam->addComponent( new CameraComponent() );
	cam->addComponent( new FollowCamComponent() );
	cam->gc<TransformComponent>()->setTranslation( glm::vec3( -5, 0, 0 ) );
	stage->addEntity(cam);

	Entity * thing = makeAThing();
	thing->addComponent( new InputMoveComponent() );
	thing->addComponent( new PlaneComponent() );
	thing->gc<PlaneComponent>()->bullet = content->loadGeometry( "Resources/shovel.mod" )->makeRenderable(passShad,cow);
	thing->getComponent<TransformComponent>()->setTranslation( glm::vec3( 0, 20, 0 ) );
	thing->gc<RenderComponent>()->setRenderable( planeGeo->makeRenderable(passShad, cow) );
	stage->addEntity(thing);

	cam->gc<FollowCamComponent>()->target = thing;
	cam->gc<FollowCamComponent>()->laxness = 20;
	cam->gc<FollowCamComponent>()->desiredOffset = glm::vec3( 0, 5, 100 );

	for( int i = 0; i <= 100; i++ )
	{
		thing = makeAThing();
		thing->gc<ColliderComponent>()->setAsBox( glm::vec3( 5, 5, 5 ) );
		thing->gc<TransformComponent>()->setScale( glm::vec3( 5, 5, 5 ) );
		thing->gc<RenderComponent>()->setRenderable( cubeGeo->makeRenderable(passShad, cow) );
		stage->addEntity(thing);
		thing->getComponent<TransformComponent>()->setTranslation( glm::vec3( 10*(i-50), 0, 0 ) );
	}
	
	glm::vec4 amb = glm::vec4(0.1f,0.1f,0.1f,1);
	Shader::setGlobalUniformParameter( "amblight", PT_VEC4, &amb );//set
	Shader::addGlobalUniformParameter( "diffpos", PT_VEC3, &lightPos );
	Shader::addGlobalUniformParameter( "difflight", PT_VEC4, &difLight);//set
	Shader::addGlobalUniformParameter( "colorInfluence", PT_VEC4, &colInfluence);//set
	Shader::addGlobalUniformParameter( "specColor", PT_VEC4, &specColor);//set
	Shader::addGlobalUniformParameter( "tightness", PT_FLOAT, &(specAmount) );
}

void PubHandle::shutdown()
{
	delete game;
}

void PubHandle::paint( Graphics* graphics )
{
	graphics->setClearColor( 0, 0.7f, 1, 1 );
	game->subscribeGraphicsHandle( graphics );
	game->draw();
}

void PubHandle::update( float dt, const UserController *uc )
{
	game->subscribeUserController( uc );
	game->update( dt );
}
