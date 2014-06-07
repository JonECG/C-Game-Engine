#include "Lab10Gui.h"
#include "PhysicsGui/VectorGraphic.h"
#include <windows.h>
#include "DebugMenus.h"

float restitutionD;

Lab10Gui::Lab10Gui()
{
	numParticles = 8;

	particles = new Particle[numParticles];
	particleGraphics = new VectorGraphic*[numParticles];

	registry = ParticleForceRegistry();
	contacts = ContactRegistry();

	restitutionD = 0.75;
}

Lab10Gui::~Lab10Gui()
{
}

void Lab10Gui::reset()
{
	for( int i = 0; i < numParticles; i++ )
	{
		particles[i].velocity = glm::vec3();
		particles[i].position = glm::vec3( 0, 1, 0 ) + glm::vec3( 0, 1.5, 0 ) * float(i);
	}

	particles[numParticles-1].velocity = glm::vec3( 2, 0, 0 );
	particles[numParticles-1].position = glm::vec3( -6, 2, 0 );
}

void Lab10Gui::rest()
{
	particles[0].velocity = glm::vec3( 0, 0, 0 );
	particles[0].position = glm::vec3( 0, 0, 0 );
}

void Lab10Gui::initialize()
{
	GuiBase::initialize( false, true );
	debugLayout->addWidget( DebugMenus::menu );

	for( int i = 0; i < numParticles; i++ )
	{
		particles[i] = Particle();
		particles[i].damping = 1.0f;
		particles[i].mass = 1;

		particleGraphics[i] = addVectorGraphic();
		particleGraphics[i]->position = particles[i].position;
		particleGraphics[i]->color = glm::vec3( 0.8f, 0, 0 );
	}

	particleGraphics[numParticles - 1]->color = glm::vec3( 0, 0.8f, 0 );

	reset();

	forceGravity = new ParticleGravity( glm::vec3( 0, -10, 0 ) );
	//forceGravity->isActive = false;
	forceKeyboard = new ParticleKeyboard( 30.0f );
	forceDrag = new ParticleDrag( 0.5f, 0.002f );
	//forceDrag->isActive = false;


	registry.add( &particles[numParticles - 1], forceKeyboard );
	for( int i = 0; i < numParticles - 1; i++ )
	{
		registry.add( &particles[i], forceGravity );
		registry.add( &particles[i], forceDrag );
	}

	DebugMenus::actButton("Reset to fall", std::bind( &Lab10Gui::reset, this ) );
	DebugMenus::slideFloat("Restitution", restitutionD, 0, 1);
}

glm::vec3 Lab10Gui::vectorFromKeyInput()
{
	glm::vec3 direction = glm::vec3( (GetAsyncKeyState(VK_RIGHT)?1:0) - (GetAsyncKeyState(VK_LEFT)?1:0), (GetAsyncKeyState(VK_UP)?1:0) - (GetAsyncKeyState(VK_DOWN)?1:0),0 );
	float length = glm::length( direction );
	if ( length > 0.01 )
		direction /= length;
	return direction;
}


void Lab10Gui::resolveCollision( float dt )
{
	
}

void Lab10Gui::newFrame()
{
	DebugMenus::update();

	registry.updateForces( dt() );

	for( int inter = 0; inter < 10; inter++ )
	{
	for( int i = 0; i < numParticles; i++ )
	{
		
		for( int j = i + 1; j < numParticles; j++ )
		{
			glm::vec3 difference = particles[i].position - particles[j].position;
			float penetration = ( particles[i].mass + particles[j].mass ) / 2 - glm::length( difference );
			if( penetration > 0 )
			{
				std::cout << "Coll" << std::endl;
				contacts.addContact( &particles[i], &particles[j], glm::normalize( difference ), penetration, restitutionD );
			}
		}

		if( particles[i].position.y <= 0.5f )
		{
			std::cout << "Coll" << std::endl;
			contacts.addContact( &particles[i], glm::vec3( 0, 1, 0 ), 0.5 - particles[i].position.y, restitutionD );
		}

		/*float wallWidth = 3.0f;

		if( particles[i].position.x >= wallWidth )
		{
			std::cout << "Coll" << std::endl;
			contacts.addContact( &particles[i], glm::vec3( -1, 0, 0 ), wallWidth - particles[i].position.x, restitutionD );
		}

		if( particles[i].position.x <= -wallWidth )
		{
			std::cout << "Coll" << std::endl;
			contacts.addContact( &particles[i], glm::vec3( 1, 0, 0 ), -wallWidth - particles[i].position.x, restitutionD );
		}*/
	}

	contacts.resolveContacts( dt() );
	}

	for( int i = 0; i < numParticles; i++ )
	{
		particles[i].update( dt() );
		particleGraphics[i]->pointSize = particles[i].mass;
	}

	glm::vec3 momentum = particles[1].velocity * particles[1].mass;

	for( int i = 0; i < numParticles; i++ )
	{
		sync( particleGraphics[i], particles[i].position );
	}
	
}

void Lab10Gui::vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta )
{
	particles[vectorGraphicIndex].position += dragDelta;
}