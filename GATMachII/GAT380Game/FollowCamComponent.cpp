#include "FollowCamComponent.h"
#include "Entity.h"
#include "Components\TransformComponent.h"
#include "Components\CameraComponent.h"
#include "Stage.h"

FollowCamComponent::FollowCamComponent()
{
	target = nullptr;
	laxness = 1;
}


void FollowCamComponent::update( float dt )
{
	dt;

	if( target != nullptr && parent->getStage()->hasEntity( target ) )
	{
		parent->getComponent<TransformComponent>()->setTranslation( ( parent->getComponent<TransformComponent>()->getTranslation()*laxness + target->getComponent<TransformComponent>()->getTranslation() + desiredOffset )/(laxness+1) );
		//parent->getComponent<CameraComponent>()->lookAt( target->getComponent<TransformComponent>()->getTranslation(), glm::vec3( 0, 1, 0 ) );
	}
}