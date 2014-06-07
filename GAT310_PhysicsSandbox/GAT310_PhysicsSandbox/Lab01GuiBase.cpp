#include "Lab01GuiBase.h"
#include "PhysicsGui/VectorGraphic.h"
#include <windows.h>

Lab01GuiBase::Lab01GuiBase()
{
	position = glm::vec3();
	velocity = glm::vec3();
}


Lab01GuiBase::~Lab01GuiBase()
{
}

void Lab01GuiBase::initialize()
{
	GuiBase::initialize( false, true );

	centralGraphic = addVectorGraphic();
	centralGraphic->color = glm::vec3( 0, 0.8f, 0 );
	centralGraphic->position = position;

	velocityGraphic = addVectorGraphic();
	velocityGraphic->color = glm::vec3( 0, 0.8f, 0 );
	velocityGraphic->displayStyle = DS_ARROW;
	velocityGraphic->position = velocity;
}

glm::vec3 Lab01GuiBase::vectorFromKeyInput()
{
	glm::vec3 direction = glm::vec3( (GetAsyncKeyState(VK_RIGHT)?1:0) - (GetAsyncKeyState(VK_LEFT)?1:0), (GetAsyncKeyState(VK_UP)?1:0) - (GetAsyncKeyState(VK_DOWN)?1:0),0 );
	float length = glm::length( direction );
	if ( length > 0.01 )
		direction /= length;
	return direction;
}

void Lab01GuiBase::newFrame()
{
	velocity += dt() * vectorFromKeyInput();
	position += velocity * dt();

	sync( centralGraphic, position );
	sync( velocityGraphic, velocity, position );
}

void Lab01GuiBase::vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta )
{
	position += dragDelta;
}