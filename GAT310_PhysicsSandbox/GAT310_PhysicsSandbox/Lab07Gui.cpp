#include "Lab07Gui.h"
#include "PhysicsGui/VectorGraphic.h"
#include <windows.h>
#include "DebugMenus.h"

float watchTotalMomentum, watchTotalEnergy, restitution, initOneVelocity, initTwoVelocity;
Lab07Gui::Lab07Gui()
{
	numParticles = 2;

	particles = new Particle[numParticles];
	particleGraphics = new VectorGraphic*[numParticles];

	registry = ParticleForceRegistry();

	initOneVelocity = 2;
	initTwoVelocity = -3;
	watchTotalEnergy = 0;
	watchTotalMomentum = 0;
	restitution = 1;
}

Lab07Gui::~Lab07Gui()
{
}

void Lab07Gui::reset()
{
	particles[0].velocity = glm::vec3();
	particles[0].velocity.x = initOneVelocity;
	particles[0].position = glm::vec3( -3, 0, 0 );

	particles[1].velocity = glm::vec3();
	particles[1].velocity.x = initTwoVelocity;
	particles[1].position = glm::vec3( 3, 0, 0 );
}

void Lab07Gui::initialize()
{
	GuiBase::initialize( false, true );
	debugLayout->addWidget( DebugMenus::menu );

	

	for( int i = 0; i < numParticles; i++ )
	{
		particles[i] = Particle();
		particles[i].damping = 1;

		particleGraphics[i] = addVectorGraphic();
		particleGraphics[i]->position = particles[i].position;
	}

	particleGraphics[0]->color = glm::vec3( 0.8f, 0, 0 );
	particleGraphics[1]->color = glm::vec3( 0, 0, 0.8f );

	particles[0].mass = 0.5;
	particles[1].mass = 0.3;
	reset();

	//particles[1].position = glm::vec3( 4.0f , 0, 0 );

	velocityOneGraphic = addVectorGraphic();
	velocityOneGraphic->color = glm::vec3( 0.3, 0, 0 );
	velocityOneGraphic->displayStyle = DS_ARROW;
	velocityOneGraphic->position = particles[0].velocity;

	velocityTwoGraphic = addVectorGraphic();
	velocityTwoGraphic->color = glm::vec3( 0, 0, 0.3f );
	velocityTwoGraphic->displayStyle = DS_ARROW;
	velocityTwoGraphic->position = particles[0].velocity;

	forceGravity = new ParticleGravity( glm::vec3( 0, -10, 0 ) );
	forceGravity->isActive = false;
	forceKeyboard = new ParticleKeyboard( 30.0f );
	forceDrag = new ParticleDrag( 0.01f, 0.01f );
	forceDrag->isActive = false;
	
	//forceSprings = new ParticleSpring*[numParticles];

	for( int i = 0; i < numParticles; i++ )
	{
		//forceSprings[ i ] = new ParticleSpring( &particles[i], 4, 40 );
	}

	for( int i = 0; i < numParticles; i++ )
	{
		/*if( i != 0 )
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
		}*/
	}

	DebugMenus::actButton("Reset Particles", std::bind( &Lab07Gui::reset, this ) );
	DebugMenus::slideFloat("Particle 1 Mass", particles[0].mass, 0.0f, 5.0f);
	DebugMenus::slideFloat("Particle 2 Mass", particles[1].mass, 0.0f, 5.0f);
	DebugMenus::slideFloat("Restitution", restitution, 0, 1);
	DebugMenus::slideFloat("Particle 1 Initial Velocity", initOneVelocity, 0, 10);
	DebugMenus::slideFloat("Particle 2 Initial Velocity", initTwoVelocity, -10, 0);
	DebugMenus::watchFloat("Particle 1 Velocity", particles[0].velocity.x);
	DebugMenus::watchFloat("Particle 2 Velocity", particles[1].velocity.x);
	DebugMenus::watchFloat("Total momentum", watchTotalMomentum);
	DebugMenus::watchFloat("Total Kinetic Energy", watchTotalEnergy);
}

glm::vec3 Lab07Gui::vectorFromKeyInput()
{
	glm::vec3 direction = glm::vec3( (GetAsyncKeyState(VK_RIGHT)?1:0) - (GetAsyncKeyState(VK_LEFT)?1:0), (GetAsyncKeyState(VK_UP)?1:0) - (GetAsyncKeyState(VK_DOWN)?1:0),0 );
	float length = glm::length( direction );
	if ( length > 0.01 )
		direction /= length;
	return direction;
}


void Lab07Gui::resolveCollision()
{
	glm::vec3 contactNormal = glm::normalize( particles[0].position - particles[1].position );
	glm::vec3 relativeVelocity = particles[0].velocity - particles[1].velocity;
	float separatingVelocity = glm::dot(relativeVelocity, contactNormal);

	// Check if it needs to be resolved.
	if (separatingVelocity > 0)
	{
		// The contact is either separating, or stationary;
		// no impulse is required.
		return;
	}
	// Calculate the new separating velocity.
	float newSepVelocity = -separatingVelocity * restitution;
	float deltaVelocity = newSepVelocity - separatingVelocity;
	// We apply the change in velocity to each object in proportion to
	// their inverse mass (i.e., those with lower inverse mass [higher
	// actual mass] get less change in velocity).
	float totalInverseMass = 1/(particles[0].mass) + 1/(particles[1].mass);

	// If all particles have infinite mass, then impulses have no effect.
	//if (totalInverseMass <= 0)
	//	return;
	// Calculate the impulse to apply.
	float impulse = deltaVelocity / totalInverseMass;
	// Find the amount of impulse per unit of inverse mass.
	glm::vec3 impulsePerIMass = contactNormal * impulse;
	// Apply impulses: they are applied in the direction of the contact,
	// and are proportional to the inverse mass.
	particles[0].velocity += impulsePerIMass /particles[0].mass;
	particles[1].velocity -= impulsePerIMass /particles[1].mass;
}

void Lab07Gui::newFrame()
{
	DebugMenus::update();

	registry.updateForces( dt() );


	if( glm::length( particles[0].position - particles[1].position ) < ( particles[0].mass + particles[1].mass )/2.0f )
	{
		std::cout << "Coll" << std::endl;
		
		resolveCollision();
	}


	for( int i = 0; i < numParticles; i++ )
	{
		particles[i].update( dt() );
		particleGraphics[i]->pointSize = particles[i].mass;
	}

	glm::vec3 momentum = particles[1].velocity * particles[1].mass;

	watchTotalMomentum = glm::length( particles[0].velocity * particles[0].mass + particles[1].velocity * particles[1].mass );
	watchTotalEnergy = ( glm::length(particles[0].velocity)*glm::length(particles[0].velocity) * particles[0].mass )/2.0f
		+ ( glm::length(particles[1].velocity)*glm::length(particles[1].velocity) * particles[1].mass )/2.0f;

	sync( velocityOneGraphic, particles[0].velocity * particles[0].mass, particles[0].position );
	sync( velocityTwoGraphic, particles[1].velocity * particles[1].mass, particles[1].position );

	for( int i = 0; i < numParticles; i++ )
	{
		sync( particleGraphics[i], particles[i].position );
	}
	
}

void Lab07Gui::vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta )
{
	//particles[vectorGraphicIndex].position += dragDelta;
}