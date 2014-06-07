#include "Lab03Gui.h"
#include "PhysicsGui/VectorGraphic.h"
#include <windows.h>
#include "DebugMenus.h"

Lab03Gui::Lab03Gui()
{
	particleGravity = Particle();
	particleGravity.position = glm::vec3( -3, 0, 0 );
	particleGravity.mass = 2;
	particleGravity.damping = 1.0f;

	particleBoth = Particle();
	particleBoth.mass = 2;
	particleBoth.damping = 1.0f;

	particleKeyboard = Particle();
	particleKeyboard.position = glm::vec3( 3, 0, 0 );
	particleKeyboard.mass = 2;
	particleKeyboard.damping = 1.0f;

	forceGravity = new ParticleGravity( glm::vec3( 0, -10, 0 ) );
	forceKeyboard = new ParticleKeyboard( 30.0f );

	registry.add( &particleGravity, forceGravity );

	registry.add( &particleBoth, forceGravity );
	registry.add( &particleBoth, forceKeyboard );

	registry.add( &particleKeyboard, forceKeyboard );
}


Lab03Gui::~Lab03Gui()
{
}

void Lab03Gui::initialize()
{
	GuiBase::initialize( false, true );

	particleGravityGraphic = addVectorGraphic();
	particleGravityGraphic->color = glm::vec3( 0, 0.8f, 0 );
	particleGravityGraphic->position = particleGravity.position;

	particleBothGraphic = addVectorGraphic();
	particleBothGraphic->color = glm::vec3( 0, 0.8f, 0 );
	particleBothGraphic->position = particleGravity.position;

	particleKeyboardGraphic = addVectorGraphic();
	particleKeyboardGraphic->color = glm::vec3( 0, 0.8f, 0 );
	particleKeyboardGraphic->position = particleGravity.position;

	velocityGraphic = addVectorGraphic();
	velocityGraphic->color = glm::vec3( 0, 0.8f, 0 );
	velocityGraphic->displayStyle = DS_ARROW;
	velocityGraphic->position = particleBoth.velocity;

	momentumGraphic = addVectorGraphic();
	momentumGraphic->color = glm::vec3( 0, 0.8f, 0 );
	momentumGraphic->displayStyle = DS_ARROW;
	momentumGraphic->position = particleBoth.velocity;
}

glm::vec3 Lab03Gui::vectorFromKeyInput()
{
	glm::vec3 direction = glm::vec3( (GetAsyncKeyState(VK_RIGHT)?1:0) - (GetAsyncKeyState(VK_LEFT)?1:0), (GetAsyncKeyState(VK_UP)?1:0) - (GetAsyncKeyState(VK_DOWN)?1:0),0 );
	float length = glm::length( direction );
	if ( length > 0.01 )
		direction /= length;
	return direction;
}

void Lab03Gui::newFrame()
{
	DebugMenus::update();

	registry.updateForces( dt() );
	particleGravity.update( dt() );
	particleBoth.update( dt() );
	particleKeyboard.update( dt() );

	glm::vec3 momentum = particleBoth.velocity * particleBoth.mass;

	sync( particleGravityGraphic, particleGravity.position );
	sync( particleBothGraphic, particleBoth.position );
	sync( particleKeyboardGraphic, particleKeyboard.position );
	sync( velocityGraphic, particleBoth.velocity, particleBoth.position );
	sync( momentumGraphic, momentum, particleBoth.position );
}

void Lab03Gui::vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta )
{
	//particle.position += dragDelta;
}