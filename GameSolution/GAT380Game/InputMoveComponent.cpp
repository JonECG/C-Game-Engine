#include "InputMoveComponent.h"
#include "Entity.h"
#include "TransformComponent.h"
#include <window.h>
InputMoveComponent::InputMoveComponent()
{
}

void InputMoveComponent::update( float dt )
{
	auto comp = parent->getComponent<TransformComponent>();
	comp->setTranslation( comp->getTranslation() + glm::vec3( 0, (GetAsyncKeyState('W')?1:0) - (GetAsyncKeyState('S')?1:0), (GetAsyncKeyState('D')?1:0) - (GetAsyncKeyState('A')?1:0) )*4.0f*dt  );
}