#include "AIPilotComponent.h"
#include <cmath>
#include "Entity.h"
#include "Components\TransformComponent.h"
#include "PlaneComponent.h"
#include "Stage.h"

AIPilotComponent::AIPilotComponent()
{
	tweenPos = glm::vec3();
}

void AIPilotComponent::update( float dt )
{
	dt;
	if( target && parent->getStage()->hasEntity( target ) )
	{
		//auto myTc = parent->gc<TransformComponent>();
		//auto tc = target->gc<TransformComponent>();
		//auto myPc = parent->gc<PlaneComponent>();
		//
		//if( tweenPos == glm::vec3() )
		//{
		//	tweenPos = tc->getTranslation();
		//}

		//float tweenAmount = 50;
		//tweenPos = ( tweenPos * tweenAmount + tc->getTranslation() ) / ( tweenAmount + 1 );

		//float neededDir = std::atan2( tweenPos.y - myTc->getTranslation().y, ( tweenPos.x - myTc->getTranslation().x ) ) * 180 / 3.1416f;
		//float actNeededDir = std::atan2( tc->getTranslation().y - myTc->getTranslation().y, ( tc->getTranslation().x - myTc->getTranslation().x ) ) * 180 / 3.1416f;
		//float angleDiff = (float) std::fmod(std::fmod(myPc->dir - neededDir, 360) + 540, 360) - 180;
		//float actAngleDiff = (float) std::fmod(std::fmod(myPc->dir - actNeededDir, 360) + 540, 360) - 180;

		////std::cout << neededDir << std::endl;

		//myPc->dir -= std::max( -100.0f, std::min( 100.0f, angleDiff ) ) * dt;

		//if( std::abs( actAngleDiff ) < 10 )
		//{
		//	myPc->fire();
		//}
	}
}