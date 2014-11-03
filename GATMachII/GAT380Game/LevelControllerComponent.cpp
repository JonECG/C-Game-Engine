#include "LevelControllerComponent.h"

#include "Stage.h"
#include "Entity.h"
#include "Components\TransformComponent.h"
#include "Components\CameraComponent.h"
#include "Components\OcculusCameraComponent.h"
#include "Components\RenderComponent.h"
#include "Components\ColliderComponent.h"
#include "InputMoveComponent.h"
#include "FollowCamComponent.h"
#include "Physics\PhysicsComponent.h"
#include "Physics\ForceGenerator.h"
#include "PlaneComponent.h"
#include "AIPilotComponent.h"
#include "GroundComponent.h"

#include <Renderer\Renderable.h>

Entity * makeAThing()
{
	Entity * ent = new Entity();
	ent->addComponent( new TransformComponent() );
	ent->addComponent( new RenderComponent() );
	ent->addComponent( new ColliderComponent() );
	ent->getComponent<ColliderComponent>()->setAsBox( glm::vec3( 3,3,3 ) );
	//ent->addComponent( new PhysicsComponent() );
	return ent;
}

void LevelControllerComponent::startGame()
{
	time = 10;
	Entity * cam = new Entity();
	cam->addComponent( new TransformComponent() );
	cam->addComponent( new CameraComponent() );
	cam->addComponent( new FollowCamComponent() );
	cam->gc<TransformComponent>()->setTranslation( glm::vec3( -5, 0, 0 ) );
	parent->getStage()->addEntity(cam);

	//Your plane
	Entity * thing = makeAThing();
	thing->addComponent( new InputMoveComponent() );
	thing->addComponent( new PlaneComponent() );
	thing->gc<PlaneComponent>()->alignment = 0;
	thing->gc<PlaneComponent>()->bullet = bullet;
	thing->getComponent<TransformComponent>()->setTranslation( glm::vec3( 0, 30, 0 ) );
	thing->gc<RenderComponent>()->setRenderable( friendlyPlane );
	parent->getStage()->addEntity(thing);

	player = thing;

	cam->gc<FollowCamComponent>()->target = thing;
	cam->gc<FollowCamComponent>()->laxness = 80;
	cam->gc<FollowCamComponent>()->desiredOffset = glm::vec3( 0, 5, 300 );
	cam->gc<CameraComponent>()->camFar = 2000;

	for( int i = 0; i < 4; i++ )
	{
		thing = makeAThing();
		thing->gc<ColliderComponent>()->setAsBox( glm::vec3( (i%2 == 0) ? 100 : 5, (i%2 == 0) ? 5 : 100, 5 ) );
		thing->gc<TransformComponent>()->setScale( glm::vec3( (i%2 == 0) ? 100 : 5 , (i%2 == 0) ? 5 : 100, 5 ) );
		thing->addComponent( new GroundComponent() );
		thing->gc<RenderComponent>()->setRenderable( ground->makeCopy() );
		parent->getStage()->addEntity(thing);
		thing->getComponent<TransformComponent>()->setTranslation( ( ( i < 2 ) ? 1.0f : -1.0f ) * glm::vec3(  ( (i%2 == 0) ? 0 : 95 ), (i%2 == 0) ? 95 : 0, 0 ) );
	}
}

LevelControllerComponent::LevelControllerComponent()
{
}
void LevelControllerComponent::init()
{
	startGame();
}
void LevelControllerComponent::update( float dt )
{
	time += dt;

	if( time > 10 )
	{
		time = 0;

		Entity * enemThing = makeAThing();
		enemThing->addComponent( new AIPilotComponent() );
		enemThing->gc<AIPilotComponent>()->target = player;
		enemThing->addComponent( new PlaneComponent() );
		enemThing->gc<PlaneComponent>()->alignment = 1;
		enemThing->gc<PlaneComponent>()->bullet = bullet;
		enemThing->getComponent<TransformComponent>()->setTranslation( glm::vec3( 30, 10, 0 ) );
		enemThing->gc<RenderComponent>()->setRenderable( enemyPlane );
		parent->getStage()->addEntity(enemThing);
	}

	/*if( !parent->getStage()->hasEntity( player ) )
	{
		parent->getStage()->clear();
		Entity * ent = new Entity();
		parent->getStage()->addEntity( ent );
		ent->addComponent( parent->removeComponent<LevelControllerComponent>() );
		startGame();
	}*/
}