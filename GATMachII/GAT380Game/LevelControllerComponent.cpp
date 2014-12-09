#include "LevelControllerComponent.h"

#include "Window\Graphics.h"
#include "Game.h"
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
#include "PlanetComponent.h"
#include "Window\UserController.h"

#include <glm\ext.hpp>

#include <Renderer\Renderable.h>

Entity * makeAThing()
{
	Entity * ent = new Entity();
	ent->addComponent( new TransformComponent() );
	ent->addComponent( new RenderComponent() );
	ent->addComponent( new ColliderComponent() );
	ent->getComponent<ColliderComponent>()->setAsSphere( 0.005f );
	//ent->addComponent( new PhysicsComponent() );
	return ent;
}

void LevelControllerComponent::startGame()
{
	canRestart = false;
	time = 10;
	planet = new Entity();
	planet->addComponent( new TransformComponent() );
	planet->addComponent( new RenderComponent() );
	planet->addComponent( new PlanetComponent() );
	parent->getStage()->addEntity( planet );

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
	thing->getComponent<TransformComponent>()->setTranslation( glm::vec3( 0, 1, 0 ) );
	thing->gc<RenderComponent>()->setRenderable( friendlyPlane );
	parent->getStage()->addEntity(thing);

	player = thing;

	cam->gc<FollowCamComponent>()->target = thing;
	cam->gc<FollowCamComponent>()->laxness = 5;
	//cam->gc<FollowCamComponent>()->desiredOffset = glm::vec3( 0, -1.0f, 0);
	cam->gc<FollowCamComponent>()->desiredDistance = 0.05f;
	cam->gc<CameraComponent>()->camFar = 2000;

	numEnemies = 0;

	for( int i = 0; i < 0; i++ )
	{
		thing = makeAThing();
		thing->addComponent( new AIPilotComponent() );
		thing->gc<AIPilotComponent>()->target = player;
		thing->addComponent( new PlaneComponent() );
		thing->gc<PlaneComponent>()->alignment = 1;
		thing->gc<PlaneComponent>()->bullet = bullet;
		glm::vec3 startPos = glm::vec3( std::rand()/float(RAND_MAX) - 0.5f, std::rand()/float(RAND_MAX) - 0.5f, std::rand()/float(RAND_MAX) - 0.5f );
		thing->getComponent<TransformComponent>()->setTranslation( glm::normalize( startPos ) );
		thing->gc<RenderComponent>()->setRenderable( enemyPlane );
		parent->getStage()->addEntity(thing);
		enemies[ numEnemies++ ] = thing;
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

	friendlyPlane->optionalTexture = planet->gc<PlanetComponent>()->noiseMap;
	enemyPlane->optionalTexture = planet->gc<PlanetComponent>()->noiseMap;

	if( time > 1000 )
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

	if( canRestart )
	{
		auto uc = parent->getStage()->getGame()->getUserController();
		if( uc->isKeyPressed( 'R' ) )
		{
			parent->getStage()->clear();
			Entity * ent = new Entity();
			parent->getStage()->addEntity( ent );
			LevelControllerComponent * cont = new LevelControllerComponent();
			cont->friendlyPlane = friendlyPlane;
			cont->enemyPlane = enemyPlane;
			cont->bullet = bullet;
			cont->ground = ground;
			cont->indicator = indicator;

			ent->addComponent( cont );
		}
	}
	else
	{
		if( !parent->getStage()->hasEntity( player ) )
		{
			canRestart = true;
			restartMessage = "You Lost... Press 'R' to restart";
		}

		bool allEnemiesDead = true;
		for( int i = 0; i < numEnemies; i++ )
		{
			if( parent->getStage()->hasEntity( enemies[i] ) )
			{
				allEnemiesDead = false;
			}
		}

		if( allEnemiesDead )
		{
			canRestart = true;
			restartMessage = "You win! Press 'R' to restart";
		}
	}
}

void LevelControllerComponent::draw()
{
	Graphics * g = parent->getStage()->getGame()->getGraphicsHandle();
	if( parent->getStage()->hasEntity( player ) )
	{
		glm::vec3 source = player->gc<TransformComponent>()->getTranslation();
		glm::vec3 up = glm::normalize( source );

		for( int i = 0; i < numEnemies; i++ )
		{
			if( parent->getStage()->hasEntity( enemies[i] ) )
			{
				glm::vec3 zaxis = glm::normalize( enemies[i]->gc<TransformComponent>()->getTranslation() - source );
				glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize( up ), zaxis));
				glm::vec3 yaxis = glm::cross(zaxis, xaxis);

				glm::mat3 lookMat = glm::mat3( zaxis, yaxis, xaxis );

				glm::mat4 actLook = glm::mat4( lookMat );
				actLook[3][3] = 1;

				g->setTransform( glm::translate( source - up * 0.002f ) * actLook * glm::scale( glm::vec3(0.02f) ) );
				g->drawRenderable( indicator );
			}
		}
	}

	if( canRestart )
	{
		parent->getStage()->getGame()->getGraphicsHandle()->setTransform( glm::mat4() );
		g->drawText( -0.5f, -0.5f, restartMessage );
	}
}