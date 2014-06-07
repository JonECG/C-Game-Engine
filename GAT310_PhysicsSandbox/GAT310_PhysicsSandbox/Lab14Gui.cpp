#include "Lab14Gui.h"
#include "PhysicsGui/VectorGraphic.h"
#include <windows.h>
#include "DebugMenus.h"
#include "Qt\qapplication.h"

float radiusc;
float angularVelocityc, totalInertia, torqueMagnitude;
glm::vec3 leverBase, force;
VectorGraphic * leverGraphic, *leverForceGraphic, *projectedLeverForceGraphic;
bool randomLengthb;
bool wasRandomLengthb;

Lab14Gui::Lab14Gui()
{
	MAXPARTICLE = 200;
	particleGraphics = new VectorGraphic*[MAXPARTICLE];
	springCount = 0;
	//numParticles = 1;s
	radiusc = 2;
	randomLengthb = true;
	wasRandomLengthb = randomLengthb;
	angularVelocityc = 2;
	forceSprings = new ParticleSpring*[10000];
	world.collisionDetection.restitution = 0.8f;
	world.collisionDetection.collideWithFloor = false;
}



Lab14Gui::~Lab14Gui()
{
}

void Lab14Gui::throwing()
{
	float angle = 6 * std::rand()/float(RAND_MAX);
}
void Lab14Gui::catching()
{
}

void Lab14Gui::reset()
{
	world.clearParticles();
	

	numParticles = 8;

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
		if( randomLengthb )
			part->radius = std::rand()/float(RAND_MAX);
		else
			part->radius = 1;
		part->damping = 1;
		part->mass = std::rand()/float(RAND_MAX);
	
		world.addParticle( part );
	}

	wasRandomLengthb = randomLengthb;
}

void Lab14Gui::makeConstraints()
{
}

void Lab14Gui::initialize()
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

	leverGraphic = addVectorGraphic();
	leverGraphic->color = glm::vec3( 0, 0, 0 );
	leverGraphic->displayStyle = DisplayStyle::DS_ARROW;

	leverForceGraphic = addVectorGraphic();
	leverForceGraphic->color = glm::vec3( 0, 0.5, 1 );
	leverForceGraphic->displayStyle = DisplayStyle::DS_ARROW;

	projectedLeverForceGraphic = addVectorGraphic();
	projectedLeverForceGraphic->color = glm::vec3( 1, 0.5, 0 );
	projectedLeverForceGraphic->displayStyle = DisplayStyle::DS_ARROW;

	DebugMenus::actButton("Reset", std::bind( &Lab14Gui::reset, this ) );
	DebugMenus::toggleBool("Use Random Length", randomLengthb );
	DebugMenus::slideFloat("Radius", radiusc, 0.1f, 10.0f);

	DebugMenus::watchFloat( "Angular Velocity", angularVelocityc );
	DebugMenus::watchFloat( "Total Inertia", totalInertia );
	DebugMenus::watchFloat( "Torque Magnitude", torqueMagnitude );
}

glm::vec3 Lab14Gui::vectorFromKeyInput()
{
	glm::vec3 direction = glm::vec3( (GetAsyncKeyState(VK_RIGHT)?1:0) - (GetAsyncKeyState(VK_LEFT)?1:0), (GetAsyncKeyState(VK_UP)?1:0) - (GetAsyncKeyState(VK_DOWN)?1:0),0 );
	float length = glm::length( direction );
	if ( length > 0.01 )
		direction /= length;
	return direction;
}

void Lab14Gui::newFrame()
{
	DebugMenus::update();

	if( randomLengthb != wasRandomLengthb )
	{
		reset();
	}

	if(QApplication::mouseButtons() == Qt::MouseButton::RightButton)
	{
		leverBase = getMousePosition();
	}

	if(QApplication::mouseButtons() == Qt::MouseButton::LeftButton)
	{
		force = getMousePosition() - leverBase;
	}

	glm::vec3 projected;
	if( glm::length( leverBase ) > 0 && glm::length( force ) > 0 )
	{
		glm::vec3 perpNorm = glm::normalize( glm::vec3( -leverBase.y, leverBase.x, leverBase.z ) );
		projected = glm::dot( perpNorm, force ) * perpNorm;
	}

	sync( leverGraphic, leverBase );
	sync( leverForceGraphic, force, leverBase );
	sync( projectedLeverForceGraphic, projected, leverBase );


	totalInertia = 0;

	for( int i = 0; i < numParticles; i++ )
	{
		totalInertia += glm::dot( world[i]->radius * radiusc, world[i]->radius * radiusc ) * world[i]->mass;
	}

	glm::vec3 torque = glm::cross( leverBase, projected );

	float sign = 1;

	if( torque.z < 0 )
	{
		sign = -1;
	}

	torqueMagnitude = glm::length( projected ) * sign * glm::dot(leverBase, leverBase);

	angularVelocityc += glm::dot(leverBase, leverBase) * dt() * sign * glm::length( projected ) / totalInertia;



	for( int i = 0; i < numParticles; i++ )
	{
		if ( radiusc != 0 )
		{
			glm::vec3 offset = world[i]->position;
			if( glm::length( world[i]->position ) == 0 )
			{
				offset = glm::vec3( 1, 0, 0 );
			}
			float currentAngle = std::atan2( offset.y, offset.x );
			currentAngle += angularVelocityc * dt();

			glm::vec3 prev = glm::normalize( offset ) * radiusc * world[i]->radius;
			glm::vec3 nextPosition = glm::vec3( std::cos( currentAngle ), std::sin( currentAngle ), 0) * radiusc * world[i]->radius;
			
			world[i]->velocity = ( nextPosition - prev ) / dt();

			world[i]->position = glm::normalize( offset )* radiusc  * world[i]->radius;
			/*
			world[0]->velocity = glm::normalize( world[0]->position ) * angularVelocity;
			float held = world[0]->velocity.x;
			world[0]->velocity.x = -world[0]->velocity.y;
			world[0]->velocity.y = held;
			*/
		}
	}

	world.update( dt() );
	

	

	for( int i = 0; i < numParticles; i++ )
	{
		particleGraphics[i*4]->pointSize = world[i]->mass;
		sync( particleGraphics[i*4], world[i]->position );
		sync( particleGraphics[i*4+1], world[i]->position );
		sync( particleGraphics[i*4+2], world[i]->velocity, world[i]->position );
		sync( particleGraphics[i*4+3], world[i]->velocity, world[i]->position );
	}

	for( int i = numParticles * 4; i < MAXPARTICLE; i++ )
	{
		sync( particleGraphics[i], glm::vec3( 10000,10000,10000) );
	}
	
}

void Lab14Gui::vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta )
{
	//world[vectorGraphicIndex]->position += dragDelta;
}