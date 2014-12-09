#include "AIPilotComponent.h"
#include <cmath>
#include "Entity.h"
#include "Components\TransformComponent.h"
#include "PlaneComponent.h"
#include "Stage.h"

AIPilotComponent::AIPilotComponent()
{
	tweenPos = glm::vec3();
	time = 0;
	following = false;
}

float getAngleDiff( glm::vec3 normal, glm::vec3 source, glm::vec3 direction, glm::vec3 target )
{
	glm::vec3 targetPlaneProjected = target - normal*glm::dot( normal, target );
	glm::vec3 destinationPlaneProjected = source + direction - normal*glm::dot( normal, source + direction );

	float angle = std::acos( glm::dot( glm::normalize( targetPlaneProjected - source ), glm::normalize( destinationPlaneProjected - source ) ) );
	glm::vec3 crossReference = glm::cross( normal, destinationPlaneProjected );
	angle *= ( glm::dot( crossReference, targetPlaneProjected ) < 0 ) ? 1 : -1;

	return angle;
}

void AIPilotComponent::update( float dt )
{
	time += dt;

	if( time > 4 )
	{
		time -= 4;
		following = float( std::rand() )/RAND_MAX > 0.4f;
	}

	if( target && parent->getStage()->hasEntity( target ) )
	{
		auto myTc = parent->gc<TransformComponent>();
		auto tc = target->gc<TransformComponent>();
		auto myPc = parent->gc<PlaneComponent>();
		
		if( tweenPos == glm::vec3() )
		{
			tweenPos = tc->getTranslation();
		}

		float tweenAmount = 20;
		if( float( std::rand() )/RAND_MAX < 0.2f )
			tweenPos = ( tweenPos * tweenAmount + tc->getTranslation() ) / ( tweenAmount + 1 );

		glm::vec3 planeNormal = glm::normalize( myTc->getTranslation() );
		float angleDiff = getAngleDiff( planeNormal, myTc->getTranslation(), myPc->dir, tweenPos );
		//float actAngleDiff = getAngleDiff( planeNormal, myTc->getTranslation(), myPc->dir, tc->getTranslation() );
		
		if( following )
		{
			myPc->turning += ( ( angleDiff > 0 ) ? 1 : -1 ) * dt * std::min( std::abs(angleDiff)/10, 0.1f );

			//std::cout << angleDiff << std::endl;
			if( std::abs( angleDiff ) < 1.0f )
			{
				myPc->fire();
			}
		}
	}
}