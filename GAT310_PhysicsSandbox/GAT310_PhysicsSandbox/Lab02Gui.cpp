#include "Lab02Gui.h"
#include "PhysicsGui/VectorGraphic.h"
#include <windows.h>
#include "DebugMenus.h"

Lab02Gui::Lab02Gui()
{
	particle = Particle();
	particle.mass = 2;
	particle.damping = 1.0f;
}


Lab02Gui::~Lab02Gui()
{
}

glm::vec3 momentum;
float momentumMagnitude;
bool conserveMomentum;

void Lab02Gui::initialize()
{
	GuiBase::initialize( false, true );

	centralGraphic = addVectorGraphic();
	centralGraphic->color = glm::vec3( 0, 0.8f, 0 );
	centralGraphic->position = particle.position;

	velocityGraphic = addVectorGraphic();
	velocityGraphic->color = glm::vec3( 0, 0.8f, 0 );
	velocityGraphic->displayStyle = DS_ARROW;
	velocityGraphic->position = particle.velocity;

	momentumGraphic = addVectorGraphic();
	momentumGraphic->color = glm::vec3( 0, 0.8f, 0 );
	momentumGraphic->displayStyle = DS_ARROW;
	momentumGraphic->position = particle.velocity;

	//DebugMenus::slideVector( "Velocity", particle.velocity, -1, 1 );
	DebugMenus::slideFloat( "Mass", particle.mass, 0.1f, 20.0f );
	DebugMenus::watchFloat( "Current Momentum", momentumMagnitude );
	DebugMenus::toggleBool( "Conserve Momentum", conserveMomentum );
}

glm::vec3 Lab02Gui::vectorFromKeyInput()
{
	glm::vec3 direction = glm::vec3( (GetAsyncKeyState(VK_RIGHT)?1:0) - (GetAsyncKeyState(VK_LEFT)?1:0), (GetAsyncKeyState(VK_UP)?1:0) - (GetAsyncKeyState(VK_DOWN)?1:0),0 );
	float length = glm::length( direction );
	if ( length > 0.01 )
		direction /= length;
	return direction;
}

void Lab02Gui::newFrame()
{
	DebugMenus::update();

	if ( conserveMomentum )
		particle.velocity = momentum / particle.mass;

	particle.acceleration = 4.0f*vectorFromKeyInput();
	particle.update( dt() );

	momentum = particle.velocity * particle.mass;
	momentumMagnitude = glm::length( momentum );


	centralGraphic->pointSize = (particle.mass+1) / 2;

	sync( centralGraphic, particle.position );
	sync( velocityGraphic, particle.velocity, particle.position );
	sync( momentumGraphic, momentum, particle.position );
}

void Lab02Gui::vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta )
{
	particle.position += dragDelta;
}