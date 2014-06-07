#include "Lab11Gui.h"
#include "PhysicsGui/VectorGraphic.h"
#include <windows.h>
#include "DebugMenus.h"

float springConstant, particleDamping;
bool useRods, wasRods;
bool useSphere, wasSphere;

int MAXPARTICLE = 200;

Lab11Gui::Lab11Gui()
{
	particleGraphics = new VectorGraphic*[MAXPARTICLE];
	springConstant = 20;
	particleDamping = 0.95f;
	springCount = 0;
	forceSprings = new ParticleSpring*[10000];
	useRods = false;
	wasRods = useRods;
	useSphere = false;
	useSphere = useRods;
	world.collisionDetection.restitution = 0.8f;
}



Lab11Gui::~Lab11Gui()
{
}

void Lab11Gui::reset()
{
	world.clearParticles();


	if( useSphere )
	{
		numParticles = 0;

		for( int i = 0; i < 5; i++ )
		{
			int turns = 10;
			for( int j = 0; j < turns; j++ )
			{
				Particle * part = new Particle();
				part->damping = 1.0f;
				part->mass = 1;
				float angle = j * 2 * 3.14159f / turns;
				part->position = glm::vec3( std::sin(angle), 1 + i/2.0f, std::cos(angle) ) * 5.0f;
				world.addParticle( part );
				numParticles++;
			}
		}
	}
	else
	{
		float cubeWidth = 3;
		numParticles = 8;

		for( int i = 0; i < numParticles; i++ )
		{
			Particle * part = new Particle();
			part->damping = 1.0f;
			part->mass = 1;
			world.addParticle( part );
		}

		world[0]->position = glm::vec3( cubeWidth, 0, -cubeWidth );
		world[1]->position = glm::vec3( -cubeWidth, 0, -cubeWidth );
		world[2]->position = glm::vec3( cubeWidth, 0, cubeWidth );
		world[3]->position = glm::vec3( -cubeWidth, 0, cubeWidth );

		world[4]->position = glm::vec3( cubeWidth, 2*cubeWidth, -cubeWidth );
		world[5]->position = glm::vec3( -cubeWidth, 2*cubeWidth, -cubeWidth );
		world[6]->position = glm::vec3( cubeWidth, 2*cubeWidth, cubeWidth );
		world[7]->position = glm::vec3( -cubeWidth, 2*cubeWidth, cubeWidth );

		for( int i = 0; i < numParticles; i++ )
		{
			world[i]->velocity = glm::vec3();
		}
	}

	wasSphere = useSphere;

	makeConstraints();
}

void Lab11Gui::makeConstraints()
{
	springCount = 0;
	world.forceRegistry.clear();
	world.collisionDetection.clearConstraints();
	if( useRods )
	{
		for( int i = 0; i < numParticles - 1; i++ )
		{
			for( int j = i + 1; j < numParticles; j++ )
			{
				if( i != j )
				{
					world.collisionDetection.addConstraint( world[i], new ParticleRod( world[j], &world.contactRegistry, glm::length( world[i]->position - world[j]->position ) ) );
				}
			} 
		}
		//world.collisionDetection.addConstraint( world[0], new ParticleRod( world[1], &world.contactRegistry, glm::length( world[1]->position - world[0]->position ) ) );
	}
	else
	{
		for( int i = 0; i < numParticles; i++ )
		{
			for( int j = 0; j < numParticles; j++ )
			{
				if( i != j )
				{
					forceSprings[springCount] = new ParticleSpring( world[j], glm::length( world[j]->position - world[i]->position ), 8 );
					world.addModifier( world[i], forceSprings[springCount] );
					springCount++;
				}
			} 
		}
	}

	for( int i = 0; i < numParticles; i++ )
	{
		world.addModifier( world[i], forceGravity );
		world.addModifier( world[i], forceDrag );
	}

	wasRods = useRods;
}

void Lab11Gui::initialize()
{
	GuiBase::initialize( true, true );
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
		particleGraphics[i]->color = glm::vec3( 0.8f, 0, 0 );
	}

	DebugMenus::actButton("Reset shape position and velocity", std::bind( &Lab11Gui::reset, this ) );
	DebugMenus::toggleBool("Use Rods", useRods );
	DebugMenus::toggleBool("Use Cylinder", useSphere );
	DebugMenus::slideFloat("Restitution", world.collisionDetection.restitution, 0, 1);
	DebugMenus::slideFloat("Damping", particleDamping, 0.9f, 1.0f);
	DebugMenus::slideFloat("Spring Constant", springConstant, 0, 100);
}

glm::vec3 Lab11Gui::vectorFromKeyInput()
{
	glm::vec3 direction = glm::vec3( (GetAsyncKeyState(VK_RIGHT)?1:0) - (GetAsyncKeyState(VK_LEFT)?1:0), (GetAsyncKeyState(VK_UP)?1:0) - (GetAsyncKeyState(VK_DOWN)?1:0),0 );
	float length = glm::length( direction );
	if ( length > 0.01 )
		direction /= length;
	return direction;
}

void Lab11Gui::newFrame()
{
	DebugMenus::update();


	if( useRods != wasRods || useSphere != wasSphere )
	{
		reset();
	}

	world.update( dt() );

	for( int i = 0; i < springCount; i++ )
	{
		forceSprings[i]->stiffness = springConstant;
	}

	for( int i = 0; i < numParticles; i++ )
	{
		world[i]->damping = particleDamping;
		particleGraphics[i]->pointSize = world[i]->mass;
	}

	for( int i = 0; i < numParticles; i++ )
	{
		sync( particleGraphics[i], world[i]->position );
	}

	for( int i = numParticles; i < MAXPARTICLE; i++ )
	{
		sync( particleGraphics[i], glm::vec3( 10000,10000,10000) );
	}
	
}

void Lab11Gui::vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta )
{
	world[vectorGraphicIndex]->position += dragDelta;
}