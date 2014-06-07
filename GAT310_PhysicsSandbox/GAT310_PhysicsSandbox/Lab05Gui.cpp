#include "Lab05Gui.h"
#include "PhysicsGui/VectorGraphic.h"
#include <windows.h>
#include "DebugMenus.h"

Lab05Gui::Lab05Gui()
{
	numParticles = 2;

	particles = new Particle[numParticles];
	particleGraphics = new VectorGraphic*[numParticles];
}


Lab05Gui::~Lab05Gui()
{
}

void Lab05Gui::initialize()
{
	GuiBase::initialize( false, true );
	debugLayout->addWidget( DebugMenus::menu );

	for( int i = 0; i < numParticles; i++ )
	{
		particles[i] = Particle();
		particles[i].mass = 2;
		particles[i].damping = 1.0f;

		particleGraphics[i] = addVectorGraphic();
		particleGraphics[i]->color = glm::vec3( 0.8f, 0, 0 );
		particleGraphics[i]->position = particles[i].position;
	}

	particles[1].position = glm::vec3( 4.0f , 0, 0 );

	velocityGraphic = addVectorGraphic();
	velocityGraphic->color = glm::vec3( 0, 0.8f, 0 );
	velocityGraphic->displayStyle = DS_ARROW;
	velocityGraphic->position = particles[0].velocity;

	momentumGraphic = addVectorGraphic();
	momentumGraphic->color = glm::vec3( 0, 0, 0.8f );
	momentumGraphic->displayStyle = DS_ARROW;
	momentumGraphic->position = particles[0].velocity;

	forceGravity = new ParticleGravity( glm::vec3( 0, -10, 0 ) );
	forceGravity->isActive = false;
	forceKeyboard = new ParticleKeyboard( 30.0f );
	forceDrag = new ParticleDrag( 0.01f, 0.01f );
	forceSpring = new ParticleSpring( &particles[0], 4, 30 );

	for( int i = 0; i < numParticles; i++ )
	{
		if( i != 0 )
		{
			registry.add( &particles[i], forceSpring );
			registry.add( &particles[i], forceGravity );
			registry.add( &particles[i], forceKeyboard );
			registry.add( &particles[i], forceDrag );
		}
	}

	//DebugMenus::slideFloat( "Coefficient (K1) Low Value", forceDrag->k1, 0.0f, 0.2f ); 
	//DebugMenus::slideFloat( "Squared Coefficient (K2) High Value", forceDrag->k2, 0.0f, 0.05f ); 

	DebugMenus::slideFloat( "Spring Length", forceSpring->length, 0.0f, 10.0f ); 
	DebugMenus::slideFloat( "Spring Stiffness", forceSpring->stiffness, 0.0f, 100.0f ); 

	DebugMenus::toggleBool( "Enable Gravity", forceGravity->isActive );
	DebugMenus::toggleBool( "Enable Keyboard", forceKeyboard->isActive );
	DebugMenus::toggleBool( "Enable Drag", forceDrag->isActive );
	DebugMenus::toggleBool( "Enable Spring", forceSpring->isActive );
}

glm::vec3 Lab05Gui::vectorFromKeyInput()
{
	glm::vec3 direction = glm::vec3( (GetAsyncKeyState(VK_RIGHT)?1:0) - (GetAsyncKeyState(VK_LEFT)?1:0), (GetAsyncKeyState(VK_UP)?1:0) - (GetAsyncKeyState(VK_DOWN)?1:0),0 );
	float length = glm::length( direction );
	if ( length > 0.01 )
		direction /= length;
	return direction;
}

void Lab05Gui::newFrame()
{
	DebugMenus::update();

	registry.updateForces( dt() );

	for( int i = 0; i < numParticles; i++ )
	{
		particles[i].update( dt() );
	}

	glm::vec3 momentum = particles[1].velocity * particles[1].mass;


	sync( velocityGraphic, particles[1].velocity, particles[1].position );
	sync( momentumGraphic, momentum, particles[1].position );

	for( int i = 0; i < numParticles; i++ )
	{
		sync( particleGraphics[i], particles[i].position );
	}
	
}

void Lab05Gui::vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta )
{
	particles[vectorGraphicIndex].position += dragDelta;
}