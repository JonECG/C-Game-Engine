#include "FollowCamComponent.h"
#include "Entity.h"
#include "TransformComponent.h"

FollowCamComponent::FollowCamComponent(void)
{
	target = nullptr;
}


void FollowCamComponent::update( float dt )
{
	if( target != nullptr )
		parent->getComponent<TransformComponent>()->setTranslation( ( parent->getComponent<TransformComponent>()->getTranslation() + target->getComponent<TransformComponent>()->getTranslation() + desiredOffset )/2.0f );
}