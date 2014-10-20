#include "InputMoveComponent.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "UserController.h"
#include "Stage.h"
#include "Game.h"
#include "PhysicsComponent.h"
#include "PlaneComponent.h"
#include <Windows.h>

InputMoveComponent::InputMoveComponent()
{
}

void InputMoveComponent::update( float dt )
{
	auto plane = parent->getComponent<PlaneComponent>();
	auto uc = parent->getStage()->getGame()->getUserController();

	plane->dir -= (uc->isKeyDown('D')?1:0) * 100 * dt;
	plane->dir += (uc->isKeyDown('A')?1:0) * 100 * dt;

	plane->speed += (uc->isKeyDown('W')?1:0) * 10 * dt;
	plane->speed -= (uc->isKeyDown('S')?1:0) * 10 * dt;

	if( uc->isKeyPressed( VK_SPACE ) )
	{
		plane->fire();
	}
	
	//auto comp = parent->getComponent<TransformComponent>();
	
}