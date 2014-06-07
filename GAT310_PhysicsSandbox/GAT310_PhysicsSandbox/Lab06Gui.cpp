#include "Lab06Gui.h"
#include "PhysicsGui/VectorGraphic.h"
#include <windows.h>
#include "DebugMenus.h"

Lab06Gui::Lab06Gui()
{
	numParticles = 12;

	particles = new Particle[numParticles];
	particleGraphics = new VectorGraphic*[numParticles];

	registry = ParticleForceRegistry();
}

float restLength = 1;
float springCoefficient = 30;
float damping = 1.0f;

Lab06Gui::~Lab06Gui()
{
}

void Lab06Gui::initialize()
{
	GuiBase::initialize( false, true );
	debugLayout->addWidget( DebugMenus::menu );

	for( int i = 0; i < numParticles; i++ )
	{
		particles[i] = Particle();
		particles[i].mass = 2;
		particles[i].damping = damping;
		particles[i].position = glm::vec3( 0.8 * i, 0 , 0);

		particleGraphics[i] = addVectorGraphic();
		particleGraphics[i]->color = glm::vec3( 0.8f, 0, 0 );
		particleGraphics[i]->position = particles[i].position;
	}

	//particles[1].position = glm::vec3( 4.0f , 0, 0 );

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
	forceDrag->isActive = false;
	
	forceSprings = new ParticleSpring*[numParticles];

	for( int i = 0; i < numParticles; i++ )
	{
		forceSprings[ i ] = new ParticleSpring( &particles[i], restLength, springCoefficient );
	}

	for( int i = 0; i < numParticles; i++ )
	{
		if( i != 0 )
		{
			registry.add( &particles[i], forceSprings[i - 1] );
			if( i != numParticles - 1 )
				registry.add( &particles[i], forceSprings[ i + 1 ] );

			registry.add( &particles[i], forceGravity );
			registry.add( &particles[i], forceDrag );
		}
		else                                                                                             
		{
			registry.add( &particles[i], forceKeyboard );
		}
	}

	//DebugMenus::slideFloat( "Coefficient (K1) Low Value", forceDrag->k1, 0.0f, 0.2f ); 
	//DebugMenus::slideFloat( "Squared Coefficient (K2) High Value", forceDrag->k2, 0.0f, 0.05f ); 

	DebugMenus::slideFloat( "Spring Length", restLength, 0.0f, 5.0f ); 
	DebugMenus::slideFloat( "Spring Stiffness", springCoefficient, 0.0f, 100.0f ); 
	DebugMenus::slideFloat( "Damping", damping, 0.9f, 1.0f ); 

	DebugMenus::toggleBool( "Enable Gravity", forceGravity->isActive );
	DebugMenus::toggleBool( "Enable Keyboard", forceKeyboard->isActive );
	DebugMenus::toggleBool( "Enable Drag", forceDrag->isActive );
	//DebugMenus::toggleBool( "Enable Spring", forceSpring->isActive );
}

glm::vec3 Lab06Gui::vectorFromKeyInput()
{
	glm::vec3 direction = glm::vec3( (GetAsyncKeyState(VK_RIGHT)?1:0) - (GetAsyncKeyState(VK_LEFT)?1:0), (GetAsyncKeyState(VK_UP)?1:0) - (GetAsyncKeyState(VK_DOWN)?1:0),0 );
	float length = glm::length( direction );
	if ( length > 0.01 )
		direction /= length;
	return direction;
}

void Lab06Gui::newFrame()
{
	DebugMenus::update();

	for( int i = 0; i < numParticles; i++ )
	{
		forceSprings[i]->stiffness = springCoefficient;
		forceSprings[i]->length = restLength;
		particles[i].damping = damping;
	}

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

void Lab06Gui::vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta )
{
	particles[vectorGraphicIndex].position += dragDelta;
}