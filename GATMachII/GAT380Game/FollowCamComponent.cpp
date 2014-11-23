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
		glm::vec3 dir = glm::normalize( parent->getComponent<TransformComponent>()->getTranslation() - target->getComponent<TransformComponent>()->getTranslation() );
		glm::vec3 wantedPosition = target->getComponent<TransformComponent>()->getTranslation() + dir * desiredDistance;
		parent->getComponent<TransformComponent>()->setTranslation( ( parent->getComponent<TransformComponent>()->getTranslation()*laxness + wantedPosition )/(laxness+1) );
		parent->getComponent<TransformComponent>()->setTranslation( glm::normalize( parent->getComponent<TransformComponent>()->getTranslation() ) * 1.1f );
		parent->gc<CameraComponent>()->lookAt( target->getComponent<TransformComponent>()->getTranslation(), glm::normalize( parent->getComponent<TransformComponent>()->getTranslation() ) );
		//parent->getComponent<TransformComponent>()->setTranslation( ( parent->getComponent<TransformComponent>()->getTranslation()*laxness + target->getComponent<TransformComponent>()->getTranslation() + desiredOffset )/(laxness+1) );
		//parent->getComponent<CameraComponent>()->lookAt( target->getComponent<TransformComponent>()->getTranslation(), glm::vec3( 0, 1, 0 ) );
	}
}