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
#include <glm\ext.hpp>

PlaneComponent::PlaneComponent()
{
	dir = glm::vec3( 0, 0, 1 );
	speed = 20;
	health = 100;
	coolDown = 0;
	maxSpeed = 0.2f;
	height = 1.03f;
	turning = 0;
}
void PlaneComponent::init()
{
	auto trans = parent->getComponent<TransformComponent>();

	if( parent->hasComponent<BulletComponent>() )
		trans->setScale( glm::vec3( 0.006f ) );
	else
		trans->setScale( glm::vec3( 0.001f ) );
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
		entity->gc<ColliderComponent>()->setAsSphere( 0.005f );
		entity->gc<PlaneComponent>()->dir = dir;
		entity->gc<PlaneComponent>()->alignment = alignment;
		entity->gc<PlaneComponent>()->speed = 0.2f+speed;
		entity->gc<PlaneComponent>()->maxSpeed = 0.2f+speed;
		coolDown = 1.0f;
	}
}
void PlaneComponent::draw()
{
	if( !parent->hasComponent<BulletComponent>() )
	{
		parent->getStage()->getGame()->getGraphicsHandle()->setTransform( glm::mat4() );
		//if( alignment == 0 )
		//	parent->getStage()->getGame()->getGraphicsHandle()->drawText( -0.95f, 0.95f, ("Health: " + std::to_string( health )).c_str(), 0.1f );
		//else
		//	parent->getStage()->getGame()->getGraphicsHandle()->drawText( -1.0f, 0.85f, ("Enemy Health: " + std::to_string( health )).c_str(), 0.1f );
	}
}
void PlaneComponent::update( float dt )
{
	if( parent->hasComponent<BulletComponent>() )
	{
		health -= dt * 30;
	}
	if ( health <= 0 )
	{
		parent->getStage()->deleteEntity( parent );
	}
	coolDown -= dt;

	turning *= 0.95f;

	speed = std::max( 0.01f, std::min( maxSpeed, speed ) );
	auto trans = parent->getComponent<TransformComponent>();

	glm::vec3 prevPos = trans->getTranslation();
	glm::vec3 intendedPos = prevPos + glm::normalize( dir + glm::cross( dir, glm::normalize( prevPos ) ) * turning ) * speed * dt;
	glm::vec3 actPos = glm::normalize( intendedPos ) * height;

	dir = glm::normalize( actPos - prevPos );

	glm::vec3 zaxis = -dir;
	glm::vec3 xaxis = glm::normalize(glm::cross(glm::normalize( actPos ), zaxis));
	glm::vec3 yaxis = glm::cross(zaxis, xaxis);

	glm::mat3 lookMat = glm::mat3( -zaxis, yaxis, xaxis );

	if( parent->hasComponent<BulletComponent>() )
		trans->setScale( glm::vec3( 0.006f ) );
	else
		trans->setScale( glm::vec3( 0.001f ) );

	trans->setRotation( lookMat );
	trans->rotate(glm::vec3( 1, 0, 0 ), turning * 450);
	//trans->setRotation( glm::vec3( 0, 0, 1 ), dir );
	

	trans->setTranslation( actPos );
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