#include "Lab15Gui.h"
#include "PhysicsGui/VectorGraphic.h"
#include <windows.h>
#include "DebugMenus.h"
#include "Qt\qapplication.h"

float springyConstant, dampingToUse, angularAcceleration, angle, angleVelocity;
VectorGraphic * torqueForce, *linearForce, *springLine;
glm::vec3 springBase;

Lab15Gui::Lab15Gui()
{
	MAXPARTICLE = 200;
	particleGraphics = new VectorGraphic*[MAXPARTICLE];
	springCount = 0;

	angle = 0;
	springyConstant = 5;
	dampingToUse = 0.99f;
	angleVelocity = 0;
	angularAcceleration = 0;
	springBase = glm::vec3( 0, -3, 0 );
	forceSprings = new ParticleSpring*[10000];
	world.collisionDetection.restitution = 0.8f;
	world.collisionDetection.collideWithFloor = false;
}



Lab15Gui::~Lab15Gui()
{
}

void Lab15Gui::throwing()
{

}
void Lab15Gui::catching()
{
}

void Lab15Gui::reset()
{
	world.clearParticles();

	numParticles = 1;

	for( int i = 0; i < numParticles; i++ )
	{
		
		Particle * part = new Particle();
		part->position = glm::vec3(0,1,0);
		part->damping = 1;
		part->mass = 1;
	
		world.addParticle( part );

		world.addModifier( world[i], mainSpring );
		world.addModifier( world[i], forceGravity );
		world.addModifier( world[i], forceDrag );
		world.addModifier( world[i], forceKeyboard );
	}
}

void Lab15Gui::makeConstraints()
{
}

void Lab15Gui::initialize()
{
	GuiBase::initialize( false, true );
	debugLayout->addWidget( DebugMenus::menu );
	
	forceGravity = new ParticleGravity( glm::vec3( 0, -20, 0 ) );
	//forceGravity->isActive = false;
	forceKeyboard = new ParticleKeyboard( 30.0f );
	forceDrag = new ParticleDrag( 0.5f, 0.002f );
	forceDrag->isActive = false;
	mainSpring = new ParticleSpring( glm::vec3( 0, 0, 0 ), 2, springyConstant );
	mainSpring->isActive;


	reset();
	
	

	for( int i = 0; i < MAXPARTICLE; i++ )
	{
		particleGraphics[i] = addVectorGraphic();
		particleGraphics[i]->position = glm::vec3();
		particleGraphics[i]->color = glm::vec3( 0.9f, 0, 0 );
	}

	for( int i = 0; i < numParticles; i++ )
	{
		particleGraphics[i]->color = glm::vec3( 0, 0, 1 );
		particleGraphics[i]->displayStyle = DisplayStyle::DS_SQUARE;
	}

	torqueForce = addVectorGraphic();
	torqueForce->color = glm::vec3( 0, 1, 0 );
	torqueForce->displayStyle = DisplayStyle::DS_ARROW;

	linearForce = addVectorGraphic();
	linearForce->color = glm::vec3( 1, 0, 0 );
	linearForce->displayStyle = DisplayStyle::DS_ARROW;

	springLine = addVectorGraphic();
	springLine->color = glm::vec3( 0, 0, 0 );
	springLine->displayStyle = DisplayStyle::DS_LINE;

	DebugMenus::actButton("Reset", std::bind( &Lab15Gui::reset, this ) );

	DebugMenus::watchFloat( "Angular Acceleration", angularAcceleration );
	DebugMenus::slideFloat( "Spring Constant (k)", springyConstant, 0, 6 );
	DebugMenus::slideFloat( "Damping", dampingToUse, 0.9, 1.0f );
}

glm::vec3 Lab15Gui::vectorFromKeyInput()
{
	glm::vec3 direction = glm::vec3( (GetAsyncKeyState(VK_RIGHT)?1:0) - (GetAsyncKeyState(VK_LEFT)?1:0), (GetAsyncKeyState(VK_UP)?1:0) - (GetAsyncKeyState(VK_DOWN)?1:0),0 );
	float length = glm::length( direction );
	if ( length > 0.01 )
		direction /= length;
	return direction;
}

void Lab15Gui::newFrame()
{
	DebugMenus::update();

	if(QApplication::mouseButtons() == Qt::MouseButton::LeftButton && getMousePosition().y < 5 )
	{
		springBase = getMousePosition();
		mainSpring->base = springBase;
	}

	for( int i = 0; i < numParticles; i++ )
	{
		world[i]->damping = dampingToUse;
	}
	mainSpring->stiffness = springyConstant;


	glm::vec3 corner = glm::vec3( std::cos( angle + 3.1416f / 4 ), std::sin( angle + 3.1416f / 4 ), 0 ) / float(std::sqrt( 2 ));
	

	glm::vec3 springForce = mainSpring->base - (corner + world[0]->position);
	springForce = glm::normalize( springForce ) * mainSpring->stiffness * ( glm::length( springForce ) - mainSpring->length );
	glm::vec3 linearForceActual = glm::dot( springForce, glm::normalize( mainSpring->base - world[0]->position ) ) * glm::normalize( mainSpring->base - world[0]->position );
	world[0]->addForce( linearForceActual * dt() );

	sync( linearForce, linearForceActual / 2.0f, world[0]->position );

	glm::vec3 projected;
	glm::vec3 perpNorm = glm::normalize( glm::vec3( -corner.y, corner.x, corner.z ) );
	projected = glm::dot( perpNorm, springForce ) * perpNorm;


	angularAcceleration = glm::dot( perpNorm, springForce );
	angleVelocity += angularAcceleration * dt();
	angleVelocity *= world[0]->damping;
	angle += angleVelocity * dt();
	


	//totalInertia = 0;

	//for( int i = 0; i < numParticles; i++ )
	//{
	//	totalInertia += glm::dot( world[i]->radius * radiusc, world[i]->radius * radiusc ) * world[i]->mass;
	//}

	//glm::vec3 torque = glm::cross( leverBase, projected );

	//float sign = 1;

	//if( torque.z < 0 )
	//{
	//	sign = -1;
	//}

	//torqueMagnitude = glm::length( projected ) * sign * glm::dot(leverBase, leverBase);

	//angularVelocityc += glm::dot(leverBase, leverBase) * dt() * sign * glm::length( projected ) / totalInertia;



	//for( int i = 0; i < numParticles; i++ )
	//{
	//	if ( radiusc != 0 )
	//	{
	//		glm::vec3 offset = world[i]->position;
	//		if( glm::length( world[i]->position ) == 0 )
	//		{
	//			offset = glm::vec3( 1, 0, 0 );
	//		}
	//		float currentAngle = std::atan2( offset.y, offset.x );
	//		currentAngle += angularVelocityc * dt();

	//		glm::vec3 prev = glm::normalize( offset ) * radiusc * world[i]->radius;
	//		glm::vec3 nextPosition = glm::vec3( std::cos( currentAngle ), std::sin( currentAngle ), 0) * radiusc * world[i]->radius;
	//		
	//		world[i]->velocity = ( nextPosition - prev ) / dt();

	//		world[i]->position = glm::normalize( offset )* radiusc  * world[i]->radius;
	//		/*
	//		world[0]->velocity = glm::normalize( world[0]->position ) * angularVelocity;
	//		float held = world[0]->velocity.x;
	//		world[0]->velocity.x = -world[0]->velocity.y;
	//		world[0]->velocity.y = held;
	//		*/
	//	}
	//}

	world.update( dt() );
	

	
	corner = world[0]->position + glm::vec3( std::cos( angle + 3.1416f / 4 ), std::sin( angle + 3.1416f / 4 ), 0 ) / float(std::sqrt( 2 ));
	sync( torqueForce, projected/2.0f, corner );
	sync( springLine, springBase - corner, corner );

	for( int i = 0; i < numParticles; i++ )
	{
		particleGraphics[i]->pointSize = world[i]->mass;
		particleGraphics[i]->rotation = -angle;
		sync( particleGraphics[i], world[i]->position );
	}

	for( int i = numParticles; i < MAXPARTICLE; i++ )
	{
		sync( particleGraphics[i], glm::vec3( 10000,10000,10000) );
	}
	
}

void Lab15Gui::vectorGraphicMouseDrag( uint vectorGraphicIndex, const glm::vec3& dragDelta )
{
	//world[vectorGraphicIndex]->position += dragDelta;
}