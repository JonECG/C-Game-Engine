#include "AiGameHandle.h"
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include "Camera.h"
#include <QtGui\QMouseEvent>
#include "ShapeGenerator.h"
#include "AStarDebugger.h"
#include "FlagBearer.h"
#include "Flag.h"

#include "States/FlagBearerStateHome.h"
#include "States/FlagBearerStateCamp.h"
#include "States/FlagBearerStateChaseEnemy.h"
#include "States/FlagBearerStateRandomMove.h"
#include "States/FlagBearerStateSeekFlag.h"

#include "DebugMenus.h"


Camera camera;
float tightness;
glm::vec3 shipPos;
float fps, delta;


GeneralGlWindow::Renderable * character;

bool isOurFlagSafe( void * context )
{
	//std::cout << "Is our Flag Safe? ";
	FlagBearer * bearer = reinterpret_cast<FlagBearer*>(context);
	return ( bearer->env->getFlag( bearer->isRed )->carrier == nullptr );
}
bool doWeHaveTheirFlag( void * context )
{
	//std::cout << "Do we have their Flag? ";
	FlagBearer * bearer = reinterpret_cast<FlagBearer*>(context);
	return ( bearer->env->getFlag( !bearer->isRed )->carrier != nullptr );
}
bool amIClosestToTheirFlag( void * context )
{
	//std::cout << "Am I closest to their Flag? ";
	FlagBearer * bearer = reinterpret_cast<FlagBearer*>(context);
	float bestLength = 1000000;
	FlagBearer * closest = nullptr;
	for( int i = 0; i < bearer->env->getCount( bearer->isRed ); i++ )
	{
		FlagBearer * team = bearer->env->getTeam( bearer->isRed );
		FlagBearer stack = *(bearer->env->getTeam( bearer->isRed ));
		FlagBearer * checker = &(bearer->env->getTeam( bearer->isRed )[i]);
		float length = glm::length2( checker->position - bearer->env->getFlag( !bearer->isRed )->position );
		if( length < bestLength )
		{
			closest = checker;
			bestLength = length;
		}
	}
	return bearer == closest;
}
bool amIClosestToTheirBase( void * context )
{
	//std::cout << "Am I closest to their base? ";
	FlagBearer * bearer = reinterpret_cast<FlagBearer*>(context);
	float bestLength = 1000;
	FlagBearer * closest = nullptr;
	for( int i = 0; i < bearer->env->getCount( bearer->isRed ); i++ )
	{
		FlagBearer * checker = &bearer->env->getTeam( bearer->isRed )[i];
		float length = glm::length2( checker->position - bearer->env->level->getNode( bearer->env->getHomeNode( !bearer->isRed ) ) );
		if( length < bestLength )
		{
			closest = checker;
			bestLength = length;
		}
	}
	return bearer == closest;
}
bool doIHaveTheirFlag( void * context )
{
	//std::cout << "Do I have their flag? ";
	FlagBearer * bearer = reinterpret_cast<FlagBearer*>(context);
	return ( bearer->env->getFlag( !bearer->isRed )->carrier == bearer );
}
bool amIClosestToEnemyCarrier( void * context )
{
	//std::cout << "Am I closest to their enemy carrier? ";
	FlagBearer * bearer = reinterpret_cast<FlagBearer*>(context);
	float bestLength = 1000;
	FlagBearer * closest = nullptr;
	for( int i = 0; i < bearer->env->getCount( bearer->isRed ); i++ )
	{
		FlagBearer * checker = &bearer->env->getTeam( bearer->isRed )[i];
		float length = glm::length2( checker->position - bearer->env->getFlag( !bearer->isRed )->position );
		if( length < bestLength )
		{
			closest = checker;
			bestLength = length;
		}
	}
	return bearer == closest;
}

void AiGameHandle::init()
{
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_DEPTH_TEST );

	textShad = addShaderInfo( "res/texture.vert", "res/texture.frag" );
	GeneralGlWindow::TextureInfo * levelTexture = addTexture( "res/metal.png" );
	GeneralGlWindow::TextureInfo * flagTexture = addTexture( "res/flag.png" );
	GeneralGlWindow::TextureInfo * flagBlueTexture = addTexture( "res/flag2.png" );

	env = new GameWorld();
	env->redBaseNode = 8;
	env->blueBaseNode = 12;

	env->level = new WayMapLevel();
	env->level->load( "res/fullMap.way", this, textShad, levelTexture );

	Neumont::ShapeData charData = Neumont::ShapeGenerator::makeArrow();
	GeneralGlWindow::GeometryInfo * charGeo = addGeometry( charData.verts, charData.numVerts, charData.indices, charData.numIndices, GL_TRIANGLES );
	
	setUpAttribs( charGeo );
	character = addRenderable( charGeo, glm::mat4(), textShad, flagTexture );


	GeneralGlWindow::GeometryInfo * flagGeo = loadFile( "res/flag.mod" );
	setUpAttribs( flagGeo );
	env->redFlag = new Flag( addRenderable( flagGeo, glm::mat4(), textShad, flagTexture ), env, true );
	env->blueFlag = new Flag( addRenderable( flagGeo, glm::mat4(), textShad, flagBlueTexture ), env, false );

	tightness = 40.0f;

	camera = Camera( float(width())/height(), 0.1, 100 );
	camera.setFrom( glm::vec3( 0,8,4) );
	camera.setTo( glm::vec3(0,0,0) );

	addUniformParameter( textShad, "mvp", PT_MAT4, (float*)&camera.mvp );
	
	setUniformParameter( textShad, "amblight", PT_VEC4, (float*)&glm::vec4(0.1f,0.1f,0.1f,1) );
	setUniformParameter( textShad, "diffpos", PT_VEC4, (float*)&glm::vec4(3.0f,6.0f,8.0f,1) );
	setUniformParameter( textShad, "difflight", PT_VEC4, (float*)&glm::vec4(0.8f,0.8f,0.8f,1));
	setUniformParameter( textShad, "specColor", PT_VEC4, (float*)&glm::vec4(0.8f,0.8f,0.8f,1));
	addUniformParameter( textShad, "tightness", PT_FLOAT, (float*)&(tightness) );
	addUniformParameter( textShad, "eye", PT_VEC4, (float*)&camera.from);

	//Make decision tree

	//Version 1
	/*Decision * ifClosestToFlagAndTeamDoesntHaveFlag = new Decision( new Decision( new FlagBearerStateSeekFlag ), new Decision( new FlagBearerStateRandomMove ), amIClosestToTheirFlag );
	Decision * ifClosestToBaseAndDontHaveFlag = new Decision( new Decision( new FlagBearerStateCamp ), new Decision( new FlagBearerStateRandomMove ), amIClosestToTheirBase );
	Decision * ifIHaveFlag = new Decision( new Decision( new FlagBearerStateHome ), ifClosestToBaseAndDontHaveFlag, doIHaveTheirFlag );
	Decision * ifWeHaveFlag = new Decision( ifIHaveFlag, ifClosestToFlagAndTeamDoesntHaveFlag, doWeHaveTheirFlag );
	
	Decision * ifClosestToEnemy = new Decision( new Decision( new FlagBearerStateChaseEnemy ), ifWeHaveFlag, amIClosestToEnemyCarrier );
	Decision * ifClosestAndFlagNotSafe = new Decision( new Decision( new FlagBearerStateCamp ), ifClosestToEnemy, amIClosestToTheirBase );
	
	Decision * rootNode = new Decision( ifWeHaveFlag, ifClosestAndFlagNotSafe, isOurFlagSafe );*/

	//Version 2
	Decision * ifClosestToEnemy = new Decision( new Decision( new FlagBearerStateChaseEnemy ), new Decision( new FlagBearerStateRandomMove ), amIClosestToEnemyCarrier );
	Decision * ifClosestToFlag = new Decision( new Decision( new FlagBearerStateSeekFlag ), new Decision( new FlagBearerStateRandomMove ), amIClosestToTheirFlag );
	Decision * ifFlagIsSafeAndDontHaveFlag = new Decision( ifClosestToFlag, ifClosestToEnemy, isOurFlagSafe );
	Decision * ifFlagIsSafeAndHaveFlag = new Decision( new Decision( new FlagBearerStateRandomMove ), ifClosestToEnemy, isOurFlagSafe );

	Decision * ifIHaveFlag = new Decision( new Decision( new FlagBearerStateHome ), ifFlagIsSafeAndHaveFlag, doIHaveTheirFlag );
	
	Decision * rootNode = new Decision( ifIHaveFlag, ifFlagIsSafeAndDontHaveFlag, doWeHaveTheirFlag );
	
	////////////////////
	int num = 2;
	env->redTeam = reinterpret_cast<FlagBearer*>(malloc( sizeof( FlagBearer ) * num ));
	env->redCount = num;
	env->blueTeam = reinterpret_cast<FlagBearer*>(malloc( sizeof( FlagBearer ) * num ));
	env->blueCount = num;
	for( int i = 0; i < num; i++ )
	{
		env->redTeam[i] = FlagBearer( addRenderable( charGeo, glm::mat4(), textShad, flagTexture ), env, rootNode, true );
		env->blueTeam[i] = FlagBearer( addRenderable( charGeo, glm::mat4(), textShad, flagBlueTexture ), env, rootNode, false );
	}

	debugger = new AStarDebugger();
	debugger->init( this, env->level, textShad, env->redTeam[0].speed, &camera, env->redTeam[0].path, env->redTeam[0].speed );
	DebugMenus::watchFloat( "Red score", env->redScore );
	DebugMenus::watchFloat( "Blue score", env->blueScore );
}

float angle = 0;

void AiGameHandle::paint()
{
	camera.setAspect( float(width())/height() );
	glViewport( 0, 0, width(), height() );
	glClearColor( 0.5, 0.1, 0.1, 0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	env->paint();

	//debugger->paint();
}

glm::vec2 mousePosition = glm::vec2();
bool mouseTracking = false;


void AiGameHandle::mousePressEvent( QMouseEvent * mEvent )
{
	if ( mEvent->button() == Qt::MouseButton::RightButton )
	{

		if( !mouseTracking )
		{
			mousePosition.x = mEvent->x()/float(width());
			mousePosition.y = mEvent->y()/float(height());
		}
		mouseTracking = true; 
	}

}

void AiGameHandle::mouseReleaseEvent( QMouseEvent * mEvent )
{
	if ( mEvent->button() == Qt::MouseButton::RightButton )
	{
		mouseTracking = false;
	}
}

void AiGameHandle::mouseMoveEvent( QMouseEvent * mEvent )
{
	if ( mouseTracking )
	{
		glm::vec2 currentPosition = glm::vec2( mEvent->x()/float(width()), mEvent->y()/float(height()));

		glm::vec2 delta = currentPosition-mousePosition;

		glm::vec3 camNorm = glm::normalize( camera.getTo()-camera.getFrom() );

		glm::vec3 rotUp = glm::cross( glm::vec3(0,1,0), camNorm );
		glm::vec3 rotRight = glm::cross( rotUp, camNorm );


		camera.setTo( camera.getFrom() + glm::rotate( glm::rotate( camNorm, delta.y*100, rotUp ), delta.x*100, rotRight ) );
		//camera.setTo( camera.getFrom() + glm::rotate( camNorm, delta.y*100, rotUp ) );
		//camera.setTo( glm::vec3( mousePosition.x, mousePosition.y, 0 ) );

		mousePosition.x = currentPosition.x;
		mousePosition.y = currentPosition.y;
	}
}

void AiGameHandle::update( float dt )
{
	env->update( dt );
	////////
	static glm::vec2 mousePosition = glm::vec2();

	fps = 1/dt;
	delta = dt;

	glm::vec3 camNorm = glm::normalize( camera.getTo()-camera.getFrom() );
	glm::vec3 strafe = glm::normalize( -glm::cross( glm::vec3(0,1,0), camNorm ) );
	glm::vec3 up = glm::normalize( glm::cross( strafe, camNorm ) );

	glm::mat3 camSpace = glm::mat3( strafe, camNorm, up );

	glm::vec3 movement;
	/*if ( GetAsyncKeyState(VK_SHIFT) )
		movement = glm::vec3( (GetAsyncKeyState('D')?1:0) - (GetAsyncKeyState('A')?1:0), 0, (GetAsyncKeyState('W')?1:0) - (GetAsyncKeyState('S')?1:0) );
	else*/
		movement = glm::vec3( (GetAsyncKeyState('D')?1:0) - (GetAsyncKeyState('A')?1:0), (GetAsyncKeyState(VK_SPACE)?1:0) - (GetAsyncKeyState(VK_SHIFT)?1:0),(GetAsyncKeyState('W')?1:0) - (GetAsyncKeyState('S')?1:0) );

	glm::vec3 worldMove = camSpace*movement * 1.0f * dt;

	camera.setTo( camera.getTo() + worldMove);
	camera.setFrom( camera.getFrom() + worldMove);

	camera.calcModelViewProjection();

	repaint();
}