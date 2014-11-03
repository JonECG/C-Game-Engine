#include "PlaneComponent.h"
#include "glm\glm.hpp"
#include "Window\Graphics.h"
#include "Entity.h"
#include "Stage.h"
#include "Game.h"
#include "Components\TransformComponent.h"
#include "Components\RenderComponent.h"
#include "Renderer\Renderable.h"
#include "BulletComponent.h"
#include "Components\ColliderComponent.h"
#include "GroundComponent.h"

PlaneComponent::PlaneComponent()
{
	dir = 0;
	speed = 20;
	health = 100;
	coolDown = 0;
	maxSpeed = 50.0f;
}
void PlaneComponent::init()
{
}
void PlaneComponent::destroy()
{
}
void PlaneComponent::fire()
{
	if( coolDown <= 0 )
	{
		Entity * entity = new Entity;
		entity->addComponent( new RenderComponent );
		entity->addComponent( new TransformComponent );
		entity->addComponent( new PlaneComponent );
		entity->addComponent( new ColliderComponent );
		entity->addComponent( new BulletComponent );
		parent->getStage()->addEntity( entity );

		entity->gc<RenderComponent>()->setRenderable( bullet->makeCopy() );
		entity->gc<TransformComponent>()->setTranslation( parent->gc<TransformComponent>()->getTranslation() );
		entity->gc<TransformComponent>()->setScale(glm::vec3(3,3,3));
		entity->gc<ColliderComponent>()->setAsBox( glm::vec3( 1, 1, 1 ) );
		entity->gc<PlaneComponent>()->dir = dir;
		entity->gc<PlaneComponent>()->alignment = alignment;
		entity->gc<PlaneComponent>()->speed = 100+speed;
		entity->gc<PlaneComponent>()->maxSpeed = 100+speed;
		coolDown = 1.0f;
	}
}
void PlaneComponent::draw()
{
	if( !parent->hasComponent<BulletComponent>() )
	{
		parent->getStage()->getGame()->getGraphicsHandle()->setTransform( glm::mat4() );
		if( alignment == 0 )
			parent->getStage()->getGame()->getGraphicsHandle()->drawText( 0.0f, 0.0f, ("Health: " + std::to_string( health )).c_str(), 0.1f );
		//else
		//	parent->getStage()->getGame()->getGraphicsHandle()->drawText( -1.0f, 0.85f, ("Enemy Health: " + std::to_string( health )).c_str(), 0.1f );
	}
}
void PlaneComponent::update( float dt )
{
	if ( health <= 0 )
	{
		parent->getStage()->deleteEntity( parent );
	}
	coolDown -= dt;
	speed = std::max( 25.0f, std::min( maxSpeed, speed ) );
	auto trans = parent->getComponent<TransformComponent>();
	//trans->setScale( glm::vec3( 0.5f, 0.5f, 0.5f ) );
	trans->setRotation( glm::vec3( 0, 0, 1 ), dir );
	trans->rotate(glm::vec3( 1, 0, 0 ), dir);
	trans->setTranslation( trans->getTranslation() + glm::vec3( std::cos(3.14f*dir/180), std::sin(3.14f*dir/180), 0 )*speed * dt);
}
void PlaneComponent::onCollide( Entity * other, glm::vec3 collisionNormal, float interpenetration )
{
	other;
	collisionNormal;
	interpenetration;
	if( other->hasComponent<BulletComponent>() && alignment != other->getComponent<PlaneComponent>()->alignment )
	{
		std::cout << "Bang";
		parent->getStage()->deleteEntity( other );
		health -= 25;
	}

	if( other->hasComponent<GroundComponent>() )
	{
		std::cout << "GroundBang";
		parent->getStage()->deleteEntity( parent );
	}
}