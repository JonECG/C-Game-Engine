#include "PlaneComponent.h"
#include "glm\glm.hpp"
#include "Graphics.h"
#include "Entity.h"
#include "Stage.h"
#include "Game.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "Renderable.h"

void PlaneComponent::init()
{
	dir = 0;
	speed = 3;
	health = 0;
}
void PlaneComponent::destroy()
{
}
void PlaneComponent::fire()
{
	Entity * entity = new Entity;
	entity->addComponent( new RenderComponent );
	entity->addComponent( new TransformComponent );
	entity->addComponent( new PlaneComponent );
	parent->getStage()->addEntity( entity );

	entity->gc<RenderComponent>()->setRenderable( bullet->makeCopy() );
	entity->gc<TransformComponent>()->setTranslation( parent->gc<TransformComponent>()->getTranslation() );
	entity->gc<TransformComponent>()->setScale(glm::vec3(3,3,3));
	entity->gc<PlaneComponent>()->dir = dir;
	entity->gc<PlaneComponent>()->speed = 100+speed;
}
void PlaneComponent::draw()
{
	//parent->getStage()->getGame()->getGraphicsHandle()->setTransform( glm::mat4() );
	//parent->getStage()->getGame()->getGraphicsHandle()->drawText( 0, 0, ("Score: " + std::to_string( health )).c_str(), 0.1f );
}
void PlaneComponent::update( float dt )
{
	dt;
	//speed = std::max( 5.0f, std::min( 25.0f, speed ) );
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
	std::cout << "Bang";
	parent->getStage()->deleteEntity( parent );
}