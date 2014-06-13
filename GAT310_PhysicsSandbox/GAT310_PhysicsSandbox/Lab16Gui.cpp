#include "Lab16Gui.h"
#include "PhysicsGui/VectorGraphic.h"
#include <windows.h>
#include "DebugMenus.h"
#include "Qt\qapplication.h"

Lab16Gui::Lab16Gui()
{
	world.collisionDetection.restitution = 1.0f;
	world.collisionDetection.collideWithFloor = true;
}

Lab16Gui::~Lab16Gui()
{
}

float rand( float min, float max )
{
	return std::rand()/float(RAND_MAX) * (max-min) + min;
}

void Lab16Gui::reset()
{
	world.clearParticles();

	numParticles = 30;

	std::rand();std::rand();std::rand();

	for( int i = 0; i < numParticles; i++ )
	{
		
		Particle * part = new Particle();
		part->position = glm::vec3(rand( -4, 4 ),rand( -4, 4 ),0);
		part->damping = 1;
		part->mass = rand( 0.1f, 0.5f );
	
		world.addParticle( part );

		world.addModifier( world[i], forceGravity );
		world.addModifier( world[i], forceDrag );
		
	}

	world.addModifier( world[0], forceKeyboard );
}

void Lab16Gui::initialize()
{
	GuiBase::initialize( false, true );
	debugLayout->addWidget( DebugMenus::menu );
	world.init( this );
	
	forceGravity = new ParticleGravity( glm::vec3( 0, -20, 0 ) );
	forceGravity->isActive = false;

	forceKeyboard = new ParticleKeyboard( 30.0f );

	forceDrag = new ParticleDrag( 0.5f, 0.002f );
	forceDrag->isActive = false;

	reset();
	
	DebugMenus::actButton("Reset", std::bind( &Lab16Gui::reset, this ) );
}

glm::vec3 Lab16Gui::vectorFromKeyInput()
{
	glm::vec3 direction = glm::vec3( (GetAsyncKeyState(VK_RIGHT)?1:0) - (GetAsyncKeyState(VK_LEFT)?1:0), (GetAsyncKeyState(VK_UP)?1:0) - (GetAsyncKeyState(VK_DOWN)?1:0),0 );
	float length = glm::length( direction );
	if ( length > 0.01 )
		direction /= length;
	return direction;
}

void Lab16Gui::newFrame()
{
	DebugMenus::update();

	static bool wasPressed = false;
	bool isPressed = (QApplication::mouseButtons() & Qt::MouseButton::LeftButton);

	if( isPressed && !wasPressed )
	{
		glm::vec3 prop = glm::normalize( getMousePosition() - world[0]->position );

		Particle * emission = new Particle();
		emission->position = world[0]->position + prop * world[0]->radius  * 1.2f;
		emission->damping = 1;
		emission->velocity = world[0]->velocity;
		emission->mass = world[0]->mass * 0.03f;
	
		world[0]->mass -= emission->mass;

		world.addParticle( emission );

		//world.addModifier( world[i], forceGravity );
		//world.addModifier( world[i], forceDrag );

		emission->addForce( prop * 5.0f * emission->mass );
		world[0]->addForce( -prop * 5.0f * emission->mass );
	}

	wasPressed = isPressed;

	world.update( dt() );

	world.getGraphic( 0 )->color = glm::vec3( 0, 1, 0 );

	for( int i = 1; i < world.getNumberOfParticles(); i++ )
	{
		if( world[i]->mass > world[0]->mass )
		{
			world.getGraphic( i )->color = glm::vec3( 1.0f, 0, 0 );
		}
		else
		{
			world.getGraphic( i )->color = glm::vec3( 0, 0, 1.0f );
		}
	}

	world.sync( this );
}

void Lab16Gui::vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta )
{
	//world[vectorGraphicIndex]->position += dragDelta;
}