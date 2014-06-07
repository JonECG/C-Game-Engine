#include "Lab13Gui.h"
#include "PhysicsGui/VectorGraphic.h"
#include <windows.h>
#include "DebugMenus.h"

float radiusb;
float angularVelocityb;
float throwSpeedb;
VectorGraphic * velocityGraphicb;
glm::vec3 origin;
glm::vec3 originVelocity;
bool randomLength;
bool wasRandomLength;

Lab13Gui::Lab13Gui()
{
	MAXPARTICLE = 200;
	particleGraphics = new VectorGraphic*[MAXPARTICLE];
	springCount = 0;
	//numParticles = 1;s
	radiusb = 2;
	randomLength = true;
	wasRandomLength = randomLength;
	angularVelocityb = 2;
	throwSpeedb = 2;
	forceSprings = new ParticleSpring*[10000];
	world.collisionDetection.restitution = 0.8f;
	world.collisionDetection.collideWithFloor = false;
}



Lab13Gui::~Lab13Gui()
{
}

void Lab13Gui::throwing()
{
	float angle = 6 * std::rand()/float(RAND_MAX);
	originVelocity = glm::vec3(std::cos(angle),std::sin(angle),0);
}
void Lab13Gui::catching()
{
}

void Lab13Gui::reset()
{
	world.clearParticles();
	

	numParticles = 8;

	origin = glm::vec3();
	originVelocity = glm::vec3();

	std::rand();
	std::rand();
	std::rand();
	std::rand();
	std::rand();
	std::rand();

	for( int i = 0; i < numParticles; i++ )
	{
		
		Particle * part = new Particle();
		float angle = 6.28f * i / float(numParticles);
		part->position = glm::vec3( std::cos(angle) , std::sin(angle) , 0 );
		if( randomLength )
			part->mass = std::rand()/float(RAND_MAX);
		else
			part->mass = 1;
		part->damping = 1;
	
		world.addParticle( part );
	}

	wasRandomLength = randomLength;
}

void Lab13Gui::makeConstraints()
{
}

void Lab13Gui::initialize()
{
	GuiBase::initialize( false, true );
	debugLayout->addWidget( DebugMenus::menu );
	
	forceGravity = new ParticleGravity( glm::vec3( 0, -20, 0 ) );
	//forceGravity->isActive = false;
	forceKeyboard = new ParticleKeyboard( 30.0f );
	forceDrag = new ParticleDrag( 0.5f, 0.002f );
	forceDrag->isActive = false;

	reset();
	
	

	for( int i = 0; i < MAXPARTICLE; i++ )
	{
		particleGraphics[i] = addVectorGraphic();
		particleGraphics[i]->position = glm::vec3();
		particleGraphics[i]->color = glm::vec3( 0.9f, 0, 0 );
	}

	for( int i = 0; i < numParticles; i++ )
	{
		particleGraphics[i*4]->color = glm::vec3( 0.9f, 0, 0 );

		particleGraphics[i*4+1]->displayStyle = DisplayStyle::DS_ARROW;
		particleGraphics[i*4+1]->color = glm::vec3( 0, 0, 0.5f );

		particleGraphics[i*4+3]->displayStyle = DisplayStyle::DS_ARROW;
		particleGraphics[i*4+3]->color = glm::vec3( 0, 0.5f, 0 );

		particleGraphics[i*4+2]->displayStyle = DisplayStyle::DS_ARROW;
		particleGraphics[i*4+2]->color = glm::vec3( 0.5f, 0, 0 );
	}

	velocityGraphicb = addVectorGraphic();
	velocityGraphicb->color = glm::vec3( 0, 0, 0 );
	velocityGraphicb->displayStyle = DisplayStyle::DS_ARROW;

	DebugMenus::actButton("Toss", std::bind( &Lab13Gui::throwing, this ) );
	DebugMenus::actButton("Reset", std::bind( &Lab13Gui::reset, this ) );
	DebugMenus::toggleBool("Use Random Length", randomLength );
	DebugMenus::slideFloat("Radius", radiusb, 0.1f, 10.0f);
	DebugMenus::slideFloat("Angular Velocity", angularVelocityb, 0, 20);
	DebugMenus::slideFloat("Throw Speed", throwSpeedb, 0, 20);
}

glm::vec3 Lab13Gui::vectorFromKeyInput()
{
	glm::vec3 direction = glm::vec3( (GetAsyncKeyState(VK_RIGHT)?1:0) - (GetAsyncKeyState(VK_LEFT)?1:0), (GetAsyncKeyState(VK_UP)?1:0) - (GetAsyncKeyState(VK_DOWN)?1:0),0 );
	float length = glm::length( direction );
	if ( length > 0.01 )
		direction /= length;
	return direction;
}

void Lab13Gui::newFrame()
{
	DebugMenus::update();

	if( randomLength != wasRandomLength )
	{
		reset();
	}

	origin += originVelocity * throwSpeedb * dt();

	for( int i = 0; i < numParticles; i++ )
	{
		world[i]->position += originVelocity * throwSpeedb * dt();
		if ( radiusb != 0 )
		{
			glm::vec3 offset = world[i]->position - origin;
			if( glm::length( world[i]->position ) == 0 )
			{
				offset = glm::vec3( 1, 0, 0 );
			}
			float currentAngle = std::atan2( offset.y, offset.x );
			currentAngle += angularVelocityb * dt();

			glm::vec3 prev = glm::normalize( offset ) * radiusb * world[i]->mass;
			glm::vec3 nextPosition = glm::vec3( std::cos( currentAngle ), std::sin( currentAngle ), 0) * radiusb * world[i]->mass;
			
			world[i]->velocity = ( nextPosition - prev ) / dt();

			world[i]->position = origin + glm::normalize( offset )* radiusb  * world[i]->mass;
			/*
			world[0]->velocity = glm::normalize( world[0]->position ) * angularVelocity;
			float held = world[0]->velocity.x;
			world[0]->velocity.x = -world[0]->velocity.y;
			world[0]->velocity.y = held;
			*/
		}
	}

	world.update( dt() );
	

	sync( velocityGraphicb, originVelocity*throwSpeedb, origin );

	for( int i = 0; i < numParticles; i++ )
	{
		sync( particleGraphics[i*4], world[i]->position );
		sync( particleGraphics[i*4+1], world[i]->position - origin, origin );
		sync( particleGraphics[i*4+2], world[i]->velocity, world[i]->position );
		sync( particleGraphics[i*4+3], world[i]->velocity+originVelocity*throwSpeedb, world[i]->position );
	}

	for( int i = numParticles * 4; i < MAXPARTICLE; i++ )
	{
		sync( particleGraphics[i], glm::vec3( 10000,10000,10000) );
	}
	
}

void Lab13Gui::vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta )
{
	//world[vectorGraphicIndex]->position += dragDelta;
}