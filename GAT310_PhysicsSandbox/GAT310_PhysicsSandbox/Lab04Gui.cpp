#include "Lab04Gui.h"
#include "PhysicsGui/VectorGraphic.h"
#include <windows.h>
#include "DebugMenus.h"

void resetParticle( Particle * particle )
{
	particle->position = glm::vec3( -5, 0, 0 );
	particle->velocity = glm::vec3( 30, 0, 0 );
}

Lab04Gui::Lab04Gui()
{
	particle = Particle();
	resetParticle( &particle );
	particle.mass = 2;
	particle.damping = 1.0f;

	forceGravity = new ParticleGravity( glm::vec3( 0, -10, 0 ) );
	forceKeyboard = new ParticleKeyboard( 30.0f );
	forceDrag = new ParticleDrag( 0, 0 );

	registry.add( &particle, forceGravity );
	registry.add( &particle, forceKeyboard );
	registry.add( &particle, forceDrag );
}


Lab04Gui::~Lab04Gui()
{
}

void Lab04Gui::initialize()
{
	GuiBase::initialize( false, true );
	debugLayout->addWidget( DebugMenus::menu );

	particleGraphic = addVectorGraphic();
	particleGraphic->color = glm::vec3( 0, 0.8f, 0 );
	particleGraphic->position = particle.position;

	velocityGraphic = addVectorGraphic();
	velocityGraphic->color = glm::vec3( 0, 0.8f, 0 );
	velocityGraphic->displayStyle = DS_ARROW;
	velocityGraphic->position = particle.velocity;

	momentumGraphic = addVectorGraphic();
	momentumGraphic->color = glm::vec3( 0, 0.8f, 0 );
	momentumGraphic->displayStyle = DS_ARROW;
	momentumGraphic->position = particle.velocity;

	DebugMenus::slideFloat( "Coefficient (K1) Low Value", forceDrag->k1, 0.0f, 0.2f ); 
	DebugMenus::slideFloat( "Squared Coefficient (K2) High Value", forceDrag->k2, 0.0f, 0.05f ); 

	DebugMenus::toggleBool( "Enable Gravity", forceGravity->isActive );
	DebugMenus::toggleBool( "Enable Keyboard", forceKeyboard->isActive );
	DebugMenus::toggleBool( "Enable Drag", forceDrag->isActive );
}

glm::vec3 Lab04Gui::vectorFromKeyInput()
{
	glm::vec3 direction = glm::vec3( (GetAsyncKeyState(VK_RIGHT)?1:0) - (GetAsyncKeyState(VK_LEFT)?1:0), (GetAsyncKeyState(VK_UP)?1:0) - (GetAsyncKeyState(VK_DOWN)?1:0),0 );
	float length = glm::length( direction );
	if ( length > 0.01 )
		direction /= length;
	return direction;
}

void Lab04Gui::newFrame()
{
	DebugMenus::update();

	registry.updateForces( dt() );
	particle.update( dt() );


	if( particle.position.x > 5 || particle.velocity.x < 0.001)
		resetParticle( &particle );

	glm::vec3 momentum = particle.velocity * particle.mass;


	sync( velocityGraphic, particle.velocity, particle.position );
	sync( momentumGraphic, momentum, particle.position );

	sync( particleGraphic, particle.position );
}

void Lab04Gui::vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta )
{
	//particle.position += dragDelta;
}