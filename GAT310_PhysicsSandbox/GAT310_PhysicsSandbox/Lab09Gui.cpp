#include "Lab09Gui.h"
#include "PhysicsGui/VectorGraphic.h"
#include <windows.h>
#include "DebugMenus.h"

float restitutionC, inVelocity, outVelocity;

Lab09Gui::Lab09Gui()
{
	numParticles = 1;

	particles = new Particle[numParticles];
	particleGraphics = new VectorGraphic*[numParticles];

	registry = ParticleForceRegistry();

	restitutionC = 0.9;
}

Lab09Gui::~Lab09Gui()
{
}

void Lab09Gui::reset()
{
	particles[0].velocity = glm::vec3( 8, 1, 0 );
	particles[0].position = glm::vec3( -3, 3, 0 );
}

void Lab09Gui::rest()
{
	particles[0].velocity = glm::vec3( 0, 0, 0 );
	particles[0].position = glm::vec3( 0, 0, 0 );
}

void Lab09Gui::initialize()
{
	GuiBase::initialize( false, true );
	debugLayout->addWidget( DebugMenus::menu );

	for( int i = 0; i < numParticles; i++ )
	{
		particles[i] = Particle();
		particles[i].damping = 1.0f;

		particleGraphics[i] = addVectorGraphic();
		particleGraphics[i]->position = particles[i].position;
	}

	particleGraphics[0]->color = glm::vec3( 0.8f, 0, 0 );

	particles[0].mass = 1;
	reset();

	forceGravity = new ParticleGravity( glm::vec3( 0, -10, 0 ) );
	//forceGravity->isActive = false;
	forceKeyboard = new ParticleKeyboard( 30.0f );
	forceDrag = new ParticleDrag( 0.5f, 0.002f );
	//forceDrag->isActive = false;


	for( int i = 0; i < numParticles; i++ )
	{
		registry.add( &particles[i], forceGravity );
		registry.add( &particles[i], forceDrag );
		registry.add( &particles[i], forceKeyboard );
	}

	DebugMenus::actButton("Reset to fall", std::bind( &Lab09Gui::reset, this ) );
	DebugMenus::actButton("Reset to rest", std::bind( &Lab09Gui::rest, this ) );
	DebugMenus::slideFloat("Restitution", restitutionC, 0, 1);
	DebugMenus::watchFloat("Pre Velocity", inVelocity);
	DebugMenus::watchFloat("Post Velocity", outVelocity);
}

glm::vec3 Lab09Gui::vectorFromKeyInput()
{
	glm::vec3 direction = glm::vec3( (GetAsyncKeyState(VK_RIGHT)?1:0) - (GetAsyncKeyState(VK_LEFT)?1:0), (GetAsyncKeyState(VK_UP)?1:0) - (GetAsyncKeyState(VK_DOWN)?1:0),0 );
	float length = glm::length( direction );
	if ( length > 0.01 )
		direction /= length;
	return direction;
}


void Lab09Gui::resolveCollision( float dt )
{
	glm::vec3 contactNormal = glm::vec3( 0, 1, 0 );
	glm::vec3 relativeVelocity = particles[0].velocity;
	float separatingVelocity = glm::dot(relativeVelocity, contactNormal);

	// Check if it needs to be resolved.
	if (separatingVelocity > 0)
	{
		// The contact is either separating, or stationary;
		// no impulse is required.
		return;
	}
	// Calculate the new separating velocity.
	float newSepVelocity = -separatingVelocity;


	
		// Check the velocity buildup due to acceleration only.
		glm::vec3 accCausedVelocity = particles[0].forceAccum / particles[0].mass;
		//if (particle[1])
		//	accCausedVelocity -= particle[1]->getAcceleration();
		float accCausedSepVelocity = glm::dot( accCausedVelocity, contactNormal );
		// If we’ve got a closing velocity due to aceleration buildup,
		// remove it from the new separating velocity.
		if (accCausedSepVelocity < 0)
		{
			newSepVelocity += accCausedSepVelocity / restitutionC;
			// Make sure we haven’t removed more than was
			// there to remove.
			if (newSepVelocity < 0)
				newSepVelocity = 0;
		}
	




	float deltaVelocity = (restitutionC * newSepVelocity - separatingVelocity);
	// We apply the change in velocity to each object in proportion to
	// their inverse mass (i.e., those with lower inverse mass [higher
	// actual mass] get less change in velocity).
	float totalInverseMass = 1/(particles[0].mass);

		float risingAmount = glm::dot( contactNormal, particles[0].forceAccum / particles[0].mass );
		if ( risingAmount > 0 )
			risingAmount = 0;
		glm::vec3 risingVelocity = contactNormal * risingAmount;
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
		particles[0].velocity -= risingVelocity;
}

void Lab09Gui::newFrame()
{
	DebugMenus::update();

	registry.updateForces( dt() );


	

	if( (particles[0].position + particles[0].velocity * dt() ).y <= 0.5f )
	{
		std::cout << "Coll" << std::endl;
		
		resolveCollision( dt() );
		particles[0].position.y = 0.5f;
	}

	inVelocity = particles[0].velocity.y;
	
	for( int i = 0; i < numParticles; i++ )
	{
		particles[i].update( dt() );
		particleGraphics[i]->pointSize = particles[i].mass;
	}

	outVelocity = particles[0].velocity.y;

	glm::vec3 momentum = particles[1].velocity * particles[1].mass;

	for( int i = 0; i < numParticles; i++ )
	{
		sync( particleGraphics[i], particles[i].position );
	}
	
}

void Lab09Gui::vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta )
{
	particles[vectorGraphicIndex].position += dragDelta;
}