#include "Lab12Gui.h"
#include "PhysicsGui/VectorGraphic.h"
#include <windows.h>
#include "DebugMenus.h"

bool held;
float radius;
float angularVelocity;
VectorGraphic * velocityGraphic;

Lab12Gui::Lab12Gui()
{
	MAXPARTICLE = 200;
	particleGraphics = new VectorGraphic*[MAXPARTICLE];
	springCount = 0;
	numParticles = 1;
	held = true;
	radius = 2;
	angularVelocity = 2;
	forceSprings = new ParticleSpring*[10000];
	world.collisionDetection.restitution = 0.8f;
	world.collisionDetection.collideWithFloor = false;
}



Lab12Gui::~Lab12Gui()
{
}

void Lab12Gui::throwing()
{
	held = false;
}
void Lab12Gui::catching()
{
	held = true;
}

void Lab12Gui::reset()
{
	world.clearParticles();
	

	Particle * part = new Particle();
	part->position = glm::vec3( 1 , 0, 0 );
	part->mass = 1;
	part->damping = 1;
	
	world.addParticle( part );

	held = true;
}

void Lab12Gui::makeConstraints()
{
}

void Lab12Gui::initialize()
{
	GuiBase::initialize( false, true );
	debugLayout->addWidget( DebugMenus::menu );
	
	forceGravity = new ParticleGravity( glm::vec3( 0, -20, 0 ) );
	//forceGravity->isActive = false;
	forceKeyboard = new ParticleKeyboard( 30.0f );
	forceDrag = new ParticleDrag( 0.5f, 0.002f );
	forceDrag->isActive = false;

	reset();
	
	velocityGraphic = addVectorGraphic();
	velocityGraphic->color = glm::vec3( 0.4f, 0, 0 );
	velocityGraphic->displayStyle = DisplayStyle::DS_ARROW;

	for( int i = 0; i < MAXPARTICLE; i++ )
	{
		particleGraphics[i] = addVectorGraphic();
		particleGraphics[i]->position = glm::vec3();
		particleGraphics[i]->color = glm::vec3( 0.8f, 0, 0 );
	}

	DebugMenus::actButton("Toss", std::bind( &Lab12Gui::throwing, this ) );
	DebugMenus::actButton("Catch", std::bind( &Lab12Gui::catching, this ) );
	DebugMenus::slideFloat("Radius", radius, 0.0f, 10.0f);
	DebugMenus::slideFloat("Angular Velocity", angularVelocity, 0, 20);
}

glm::vec3 Lab12Gui::vectorFromKeyInput()
{
	glm::vec3 direction = glm::vec3( (GetAsyncKeyState(VK_RIGHT)?1:0) - (GetAsyncKeyState(VK_LEFT)?1:0), (GetAsyncKeyState(VK_UP)?1:0) - (GetAsyncKeyState(VK_DOWN)?1:0),0 );
	float length = glm::length( direction );
	if ( length > 0.01 )
		direction /= length;
	return direction;
}

void Lab12Gui::newFrame()
{
	DebugMenus::update();

	if( held )
	{
		if ( radius != 0 )
		{
			if( glm::length( world[0]->position ) == 0 )
			{
				world[0]->position = glm::vec3( 1, 0, 0 );
			}
			float currentAngle = std::atan2( world[0]->position.y, world[0]->position.x );
			currentAngle += angularVelocity * dt();

			glm::vec3 prev = glm::normalize( world[0]->position ) * radius;
			glm::vec3 nextPosition = glm::vec3( std::cos( currentAngle ), std::sin( currentAngle ), 0) * radius;
			
			world[0]->velocity = ( nextPosition - prev ) / dt();

			world[0]->position = glm::normalize( world[0]->position )* radius;
			/*
			world[0]->velocity = glm::normalize( world[0]->position ) * angularVelocity;
			float held = world[0]->velocity.x;
			world[0]->velocity.x = -world[0]->velocity.y;
			world[0]->velocity.y = held;
			*/
		}
		else
		{
			world[0]->position = glm::vec3();
			world[0]->velocity = glm::vec3();
		}
	}

	world.update( dt() );

	sync( velocityGraphic, world[0]->velocity, world[0]->position );

	for( int i = 0; i < numParticles; i++ )
	{
		sync( particleGraphics[i], world[i]->position );
	}

	for( int i = numParticles; i < MAXPARTICLE; i++ )
	{
		sync( particleGraphics[i], glm::vec3( 10000,10000,10000) );
	}
	
}

void Lab12Gui::vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta )
{
	world[vectorGraphicIndex]->position += dragDelta;
}