#include "InputMoveComponent.h"
#include "Entity.h"
#include "Components\TransformComponent.h"
#include "Window\UserController.h"
#include "Stage.h"
#include "Game.h"
#include "Physics\PhysicsComponent.h"
#include "PlaneComponent.h"
#include <Windows.h>

InputMoveComponent::InputMoveComponent()
{
}

void InputMoveComponent::update( float dt )
{
	auto plane = parent->getComponent<PlaneComponent>();
	auto uc = parent->getStage()->getGame()->getUserController();

	/*plane->dir -= (uc->isKeyDown('D')?1:0) * 150 * dt;
	plane->dir += (uc->isKeyDown('A')?1:0) * 150 * dt;*/

	plane->turning += (uc->isKeyDown('D')?1:0) * dt * 0.1f;
	plane->turning -= (uc->isKeyDown('A')?1:0) * dt * 0.1f;

	plane->speed += (uc->isKeyDown('W')?1:0) * 10 * dt;
	plane->speed -= (uc->isKeyDown('S')?1:0) * 10 * dt;

	if( uc->isKeyPressed( VK_SPACE ) )
	{
		plane->fire();
	}
	
	//auto comp = parent->getComponent<TransformComponent>();
	
}