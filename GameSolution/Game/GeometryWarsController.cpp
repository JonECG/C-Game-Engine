#include "GeometryWarsController.h"
#include "ParticleEffectHelper.h"
#include "StripShape.h"
#include "ChaseEnemy.h"
#include "TankEnemy.h"
#include "ListShape.h"
#include <time.h>
#include "assertions.h"

const float GeometryWarsController::PI = 3.14159265f;

Shape* GeometryWarsController::wall = nullptr; //Shape( 5, Vector2( SCREENWIDTH*0.06f, SCREENHEIGHT*0.06f ), Vector2( SCREENWIDTH*0.02f, SCREENHEIGHT*0.7f ), Vector2( SCREENWIDTH*0.2f, SCREENHEIGHT*0.95f ), Vector2( SCREENWIDTH*0.9f, SCREENHEIGHT*0.9f ), Vector2( SCREENWIDTH*0.95f, SCREENHEIGHT*0.03f ) ); 
Shape* GeometryWarsController::hourGlass = nullptr; //Shape( 6, Vector2(-1, -1), Vector2(1, -1), Vector2(0.5f, 0), Vector2(1, 1), Vector2(-1, 1), Vector2(-0.5f, 0) );
Shape* GeometryWarsController::bombIndicator = nullptr; //Shape( 4, Vector2( 32, 0 ), Vector2( 0, 32 ), Vector2( -32, 0 ), Vector2( 0, -32 ) );

using std::vector;

GeometryWarsController::GeometryWarsController()
{
	srand((int)time(NULL));
	memState = _CrtMemState();
	boss = Secret();
	stars = Starfield();
	recursiveRotate = 0;
	enemyCounter = 0;
	respawnTimer = 2.5f;
	currentCollision = 3;
	state = MENU;
	lives = 5;
	highScore = 0;
	bombs = 3;
	reviveTime = 0;
	parts = new ParticleEffect();
	enemies = new EnemyBatch();

	axis = MathHelper::randomUnit3DVector();
	rot3D = axis.length();
	rot3D = 0;
}

GeometryWarsController::~GeometryWarsController()
{
	delete parts;
	delete enemies;
}

void GeometryWarsController::init()
{
	ship = Ship();

	Core::Init( "GEOMETRY WARS", SCREENWIDTH, SCREENHEIGHT );

	ASSSERT( GeometryWarsController::wall == nullptr, "Memory clean check" );

	GeometryWarsController::wall = new StripShape( 4, Vector2( 0, 0 ), Vector2( 0, float(SCREENHEIGHT) ), Vector2( float(SCREENWIDTH), float(SCREENHEIGHT) ), Vector2( float(SCREENWIDTH), 0 ) ); 
	GeometryWarsController::hourGlass = new StripShape( 6, Vector2(-1, -1), Vector2(1, -1), Vector2(0.5f, 0), Vector2(1, 1), Vector2(-1, 1), Vector2(-0.5f, 0) );
	GeometryWarsController::bombIndicator = new StripShape( 4, Vector2( 32, 0 ), Vector2( 0, 32 ), Vector2( -32, 0 ), Vector2( 0, -32 ) );

	Ship::shipShape = new StripShape(73,Vector2(30.4f,0.1f),Vector2(13.5f,5.5f),Vector2(6.9f,13.1f),Vector2(-3.1f,19.9f),Vector2(-10.8f,18.5f),Vector2(-8.5f,10.1f),Vector2(-9.6f,7.0f),Vector2(-12.9f,7.3f),Vector2(-17.6f,8.3f),Vector2(-20.9f,11.1f),Vector2(-24.5f,15.0f),Vector2(-24.6f,12.8f),Vector2(-30.7f,14.0f),Vector2(-29.9f,2.7f),Vector2(-24.6f,3.6f),Vector2(-25.1f,0.7f),Vector2(-25.2f,-0.6f),Vector2(-23.1f,-3.4f),Vector2(-9.2f,-3.7f),Vector2(-6.8f,-0.4f),Vector2(-5.1f,-4.8f),Vector2(-4.9f,-9.0f),Vector2(-5.1f,-12.9f),Vector2(-1.0f,-14.0f),Vector2(8.1f,-5.3f),Vector2(4.5f,-4.7f),Vector2(1.9f,-3.2f),Vector2(0.9f,-0.3f),Vector2(8.2f,-5.2f),Vector2(10.8f,-4.7f),Vector2(12.9f,-3.6f),Vector2(14.2f,-2.2f),Vector2(14.9f,-0.4f),Vector2(8.1f,5.1f),Vector2(4.2f,6.7f),Vector2(0.1f,6.5f),Vector2(-3.4f,3.3f),Vector2(-3.4f,-3.3f),Vector2(0.1f,-6.5f),Vector2(4.2f,-6.7f),Vector2(8.1f,-5.1f),Vector2(14.9f,0.4f),Vector2(14.2f,2.2f),Vector2(12.9f,3.6f),Vector2(10.8f,4.7f),Vector2(8.2f,5.2f),Vector2(0.9f,0.3f),Vector2(1.9f,3.2f),Vector2(4.5f,4.7f),Vector2(8.1f,5.3f),Vector2(-1.0f,14.0f),Vector2(-5.1f,12.9f),Vector2(-4.9f,9.0f),Vector2(-5.1f,4.8f),Vector2(-6.8f,0.4f),Vector2(-9.2f,3.7f),Vector2(-23.1f,3.4f),Vector2(-25.2f,0.6f),Vector2(-25.1f,-0.7f),Vector2(-24.6f,-3.6f),Vector2(-29.9f,-2.7f),Vector2(-30.7f,-14.0f),Vector2(-24.6f,-12.8f),Vector2(-24.5f,-15.0f),Vector2(-20.9f,-11.1f),Vector2(-17.6f,-8.3f),Vector2(-12.9f,-7.3f),Vector2(-9.6f,-7.0f),Vector2(-8.5f,-10.1f),Vector2(-10.8f,-18.5f),Vector2(-3.1f,-19.9f),Vector2(6.9f,-13.1f),Vector2(13.5f,-5.5f));
	Ship::turretShape = new StripShape(15,Vector2(31.5f,0.1f),Vector2(31.5f,-2.1f),Vector2(3.5f,-2.1f),Vector2(0.9f,-7.9f),Vector2(-2.3f,-5.3f),Vector2(-0.7f,-3.1f),Vector2(-1.7f,-1.7f),Vector2(-2.9f,-0.5f),Vector2(-2.9f,0.5f),Vector2(-1.7f,1.7f),Vector2(-0.7f,3.1f),Vector2(-2.3f,5.3f),Vector2(0.9f,7.9f),Vector2(3.5f,2.1f),Vector2(31.5f,2.1f));

	Bullet::shape = new StripShape(19,Vector2(10.8f,0),Vector2(8.2f,-2.5f),Vector2(3.0f,-3.3f),Vector2(-2.8f,-3.1f),Vector2(-4.6f,-2.3f),Vector2(-6.4f,-3.9f),Vector2(-9.2f,-3.7f),Vector2(-7.2f,-2.3f),Vector2(-6.6f,-0.9f),Vector2(-8.0f,-0.7f),Vector2(-8.0f,0.7f),Vector2(-6.6f,0.9f),Vector2(-7.2f,2.3f),Vector2(-9.2f,3.7f),Vector2(-6.4f,3.9f),Vector2(-4.6f,2.3f),Vector2(-2.8f,3.1f),Vector2(3.0f,3.3f),Vector2(8.2f,2.5f));

	Starfield::star = new StripShape(8,Vector2(32.0f,0.0f),Vector2(12.0f,12.0f),Vector2(0.0f,32.0f),Vector2(-12.0f,12.0f),Vector2(-32.0f,0.0f),Vector2(-12.0f,-12.0f),Vector2(0.0f,-32.0f),Vector2(12.0f,-12.0f));
	Starfield::star->scale( 0.03f );

	Secret::jamie = new ListShape(254,Vector2(-19.5f,-1.8f),Vector2(-18.7f,5.8f),Vector2(-18.7f,5.8f),Vector2(-18.1f,12.6f),Vector2(-18.1f,12.6f),Vector2(-14.9f,17.4f),Vector2(-14.9f,17.4f),Vector2(-10.3f,18.8f),Vector2(-10.3f,18.8f),Vector2(-7.1f,18.0f),Vector2(-7.1f,18.0f),Vector2(-2.9f,19.6f),Vector2(-2.9f,19.6f),Vector2(3.3f,18.8f),Vector2(3.3f,18.8f),Vector2(7.7f,17.4f),Vector2(7.7f,17.4f),Vector2(7.5f,11.2f),Vector2(7.5f,11.2f),Vector2(8.5f,9.4f),Vector2(8.5f,9.4f),Vector2(11.5f,6.2f),Vector2(11.5f,6.2f),Vector2(13.1f,0.6f),Vector2(13.1f,0.6f),Vector2(14.3f,0.6f),Vector2(14.3f,0.6f),Vector2(17.9f,2.2f),Vector2(17.9f,2.2f),Vector2(19.9f,0.8f),Vector2(19.9f,0.8f),Vector2(20.9f,-2.8f),Vector2(20.9f,-2.8f),Vector2(18.9f,-5.6f),Vector2(18.9f,-5.6f),Vector2(17.3f,-8.6f),Vector2(17.3f,-8.6f),Vector2(17.1f,-10.6f),Vector2(17.1f,-10.6f),Vector2(14.3f,-10.6f),Vector2(14.3f,-10.6f),Vector2(13.5f,-9.6f),Vector2(15.7f,-10.6f),Vector2(11.5f,-19.8f),Vector2(11.5f,-19.8f),Vector2(2.5f,-26.6f),Vector2(2.5f,-26.6f),Vector2(-3.9f,-27.8f),Vector2(-3.9f,-27.8f),Vector2(-10.7f,-25.6f),Vector2(-10.7f,-25.6f),Vector2(-16.7f,-16.6f),Vector2(-16.7f,-16.6f),Vector2(-19.3f,-9.0f),Vector2(-19.3f,-9.0f),Vector2(-19.5f,-2.0f),Vector2(-19.5f,-2.0f),Vector2(-20.3f,-1.4f),Vector2(-20.3f,-1.4f),Vector2(-20.5f,7.2f),Vector2(-20.5f,7.2f),Vector2(-21.3f,11.8f),Vector2(-21.3f,11.8f),Vector2(-20.3f,14.6f),Vector2(-20.3f,14.6f),Vector2(-21.5f,14.8f),Vector2(-21.5f,14.8f),Vector2(-19.3f,16.8f),Vector2(-19.3f,16.8f),Vector2(-20.1f,19.4f),Vector2(-20.1f,19.4f),Vector2(-18.3f,21.6f),Vector2(-18.3f,21.6f),Vector2(-15.7f,23.8f),Vector2(-15.7f,23.8f),Vector2(-14.1f,23.6f),Vector2(-14.1f,23.6f),Vector2(-13.1f,25.4f),Vector2(-13.1f,25.4f),Vector2(-9.7f,27.4f),Vector2(-9.7f,27.4f),Vector2(-8.5f,26.6f),Vector2(-8.5f,26.6f),Vector2(-7.3f,28.0f),Vector2(-7.3f,28.0f),Vector2(-5.3f,28.4f),Vector2(-5.3f,28.4f),Vector2(-3.9f,28.8f),Vector2(-3.9f,28.8f),Vector2(-0.1f,29.2f),Vector2(-0.1f,29.2f),Vector2(3.7f,28.6f),Vector2(3.7f,28.6f),Vector2(10.5f,26.2f),Vector2(10.5f,26.2f),Vector2(12.9f,24.2f),Vector2(12.9f,24.2f),Vector2(13.7f,23.2f),Vector2(13.7f,23.2f),Vector2(15.9f,21.8f),Vector2(15.9f,21.8f),Vector2(17.9f,19.6f),Vector2(17.9f,19.6f),Vector2(17.7f,18.8f),Vector2(17.7f,18.8f),Vector2(18.7f,18.2f),Vector2(18.7f,18.2f),Vector2(18.9f,16.2f),Vector2(18.9f,16.2f),Vector2(19.5f,14.6f),Vector2(19.5f,14.6f),Vector2(18.7f,14.8f),Vector2(18.7f,14.8f),Vector2(19.7f,12.4f),Vector2(19.7f,12.4f),Vector2(19.5f,9.0f),Vector2(19.5f,9.0f),Vector2(18.9f,9.6f),Vector2(18.9f,9.6f),Vector2(18.9f,7.8f),Vector2(18.9f,7.8f),Vector2(18.9f,4.2f),Vector2(18.9f,4.2f),Vector2(18.1f,2.2f),Vector2(-15.9f,1.2f),Vector2(-14.7f,2.2f),Vector2(-14.7f,2.2f),Vector2(-11.7f,2.8f),Vector2(-11.7f,2.8f),Vector2(-10.3f,1.8f),Vector2(-10.3f,1.8f),Vector2(-13.1f,1.0f),Vector2(-13.1f,1.0f),Vector2(-15.9f,1.4f),Vector2(-13.9f,2.2f),Vector2(-14.3f,1.6f),Vector2(-14.3f,1.6f),Vector2(-13.3f,1.2f),Vector2(-13.3f,1.2f),Vector2(-12.3f,1.8f),Vector2(-12.3f,1.8f),Vector2(-12.9f,2.4f),Vector2(-12.9f,2.4f),Vector2(-13.9f,2.2f),Vector2(-2.7f,2.0f),Vector2(-0.9f,3.0f),Vector2(-0.9f,3.0f),Vector2(1.5f,3.2f),Vector2(1.5f,3.2f),Vector2(3.1f,2.4f),Vector2(3.1f,2.4f),Vector2(0.5f,1.6f),Vector2(0.5f,1.6f),Vector2(-2.7f,2.0f),Vector2(-0.3f,3.0f),Vector2(-0.9f,2.2f),Vector2(-0.9f,2.2f),Vector2(0.3f,1.8f),Vector2(0.3f,1.8f),Vector2(1.3f,2.6f),Vector2(1.3f,2.6f),Vector2(0.7f,3.2f),Vector2(0.7f,3.2f),Vector2(-0.1f,3.0f),Vector2(-12.1f,-12.6f),Vector2(-8.9f,-14.8f),Vector2(-8.9f,-14.8f),Vector2(-4.3f,-14.8f),Vector2(-4.3f,-14.8f),Vector2(-0.1f,-13.0f),Vector2(-0.1f,-13.0f),Vector2(0.1f,-12.2f),Vector2(0.1f,-12.2f),Vector2(-5.7f,-11.6f),Vector2(-5.7f,-11.6f),Vector2(-9.5f,-11.8f),Vector2(-9.5f,-11.8f),Vector2(-12.1f,-12.4f),Vector2(13.9f,-6.0f),Vector2(14.7f,-4.4f),Vector2(14.7f,-4.4f),Vector2(14.5f,-2.8f),Vector2(14.5f,-2.8f),Vector2(16.1f,-0.2f),Vector2(16.1f,-0.2f),Vector2(18.3f,1.0f),Vector2(18.3f,1.0f),Vector2(19.1f,-0.6f),Vector2(19.1f,-0.6f),Vector2(19.3f,-2.4f),Vector2(14.9f,-6.8f),Vector2(15.3f,-4.8f),Vector2(15.3f,-4.8f),Vector2(16.7f,-5.8f),Vector2(16.7f,-5.8f),Vector2(17.3f,-3.4f),Vector2(17.3f,-3.4f),Vector2(16.3f,-2.6f),Vector2(-4.1f,-3.2f),Vector2(-2.1f,-4.8f),Vector2(-2.1f,-4.8f),Vector2(-2.9f,-6.8f),Vector2(-2.9f,-6.8f),Vector2(-4.5f,-7.0f),Vector2(-4.5f,-7.0f),Vector2(-5.5f,-6.2f),Vector2(-5.5f,-6.2f),Vector2(-6.5f,-6.4f),Vector2(-6.5f,-6.4f),Vector2(-6.9f,-7.4f),Vector2(-6.9f,-7.4f),Vector2(-8.3f,-7.8f),Vector2(-8.3f,-7.8f),Vector2(-9.5f,-7.8f),Vector2(-9.5f,-7.8f),Vector2(-10.1f,-7.0f),Vector2(-10.1f,-7.0f),Vector2(-10.9f,-6.8f),Vector2(-10.9f,-6.8f),Vector2(-11.1f,-7.8f),Vector2(-11.1f,-7.8f),Vector2(-11.9f,-7.0f),Vector2(-11.9f,-7.0f),Vector2(-11.7f,-5.6f),Vector2(-11.7f,-5.6f),Vector2(-10.7f,-4.0f),Vector2(-10.7f,-4.0f),Vector2(-9.7f,-0.4f),Vector2(-9.7f,-0.4f),Vector2(-9.5f,3.4f),Vector2(-9.5f,3.4f),Vector2(-11.1f,5.0f),Vector2(-11.1f,5.0f),Vector2(-14.9f,5.6f),Vector2(-14.9f,5.6f),Vector2(-17.7f,4.6f),Vector2(-17.7f,4.6f),Vector2(-15.1f,6.4f),Vector2(-15.1f,6.4f),Vector2(-11.9f,6.6f),Vector2(-11.9f,6.6f),Vector2(-9.7f,4.4f),Vector2(-9.7f,4.4f),Vector2(-9.3f,3.2f),Vector2(-4.3f,5.8f),Vector2(-0.5f,6.4f),Vector2(-0.5f,6.4f),Vector2(3.7f,5.8f),Vector2(3.7f,5.8f),Vector2(1.3f,7.6f),Vector2(1.3f,7.6f),Vector2(-3.7f,7.2f),Vector2(-3.7f,7.2f),Vector2(-4.3f,5.8f));
	Secret::jamie->scale(8);
	Secret::jamie->flipY();

	ChaseEnemy::shape = new StripShape(87,Vector2(16.9f,-0.3f),Vector2(16.9f,-2.9f),Vector2(18.1f,-6.3f),Vector2(20.5f,-8.5f),Vector2(25.9f,-13.1f),Vector2(25.7f,-16.5f),Vector2(22.9f,-19.5f),Vector2(16.7f,-20.5f),Vector2(13.3f,-21.5f),Vector2(9.3f,-22.1f),Vector2(5.1f,-21.1f),Vector2(3.9f,-19.9f),Vector2(2.5f,-18.3f),Vector2(0.1f,-17.5f),Vector2(-2.7f,-18.5f),Vector2(-6.1f,-20.9f),Vector2(-8.1f,-22.3f),Vector2(-10.9f,-24.3f),Vector2(-15.3f,-25.7f),Vector2(-17.1f,-26.7f),Vector2(-20.1f,-28.5f),Vector2(-23.1f,-27.7f),Vector2(-23.5f,-25.5f),Vector2(-23.1f,-21.9f),Vector2(-20.1f,-15.1f),Vector2(-17.7f,-9.1f),Vector2(-25.1f,-14.9f),Vector2(-24.9f,-4.3f),Vector2(-22.1f,-4.3f),Vector2(-24.1f,-1.1f),Vector2(-22.1f,4.3f),Vector2(-17.7f,9.1f),Vector2(16.9f,2.9f),Vector2(8.9f,4.5f),Vector2(-2.3f,4.1f),Vector2(-3.9f,0.9f),Vector2(-2.3f,-3.9f),Vector2(7.3f,-4.7f),Vector2(16.9f,-0.3f),Vector2(15.3f,3.3f),Vector2(17.1f,7.1f),Vector2(25.7f,16.5f),Vector2(2.7f,18.3f),Vector2(-3.7f,-0.1f),Vector2(-3.7f,0.1f),Vector2(2.7f,-18.3f),Vector2(25.7f,-16.5f),Vector2(17.1f,-7.1f),Vector2(15.3f,-3.3f),Vector2(16.9f,0.3f),Vector2(7.3f,4.7f),Vector2(-2.3f,3.9f),Vector2(-3.9f,-0.9f),Vector2(-2.3f,-4.1f),Vector2(8.9f,-4.5f),Vector2(16.9f,-2.9f),Vector2(-17.7f,-9.1f),Vector2(-22.1f,-4.3f),Vector2(-24.1f,1.1f),Vector2(-22.1f,4.3f),Vector2(-24.9f,4.3f),Vector2(-25.1f,14.9f),Vector2(-17.7f,9.1f),Vector2(-20.1f,15.1f),Vector2(-23.1f,21.9f),Vector2(-23.5f,25.5f),Vector2(-23.1f,27.7f),Vector2(-20.1f,28.5f),Vector2(-17.1f,26.7f),Vector2(-15.3f,25.7f),Vector2(-10.9f,24.3f),Vector2(-8.1f,22.3f),Vector2(-6.1f,20.9f),Vector2(-2.7f,18.5f),Vector2(0.1f,17.5f),Vector2(2.5f,18.3f),Vector2(3.9f,19.9f),Vector2(5.1f,21.1f),Vector2(9.3f,22.1f),Vector2(13.3f,21.5f),Vector2(16.7f,20.5f),Vector2(22.9f,19.5f),Vector2(25.7f,16.5f),Vector2(25.9f,13.1f),Vector2(20.5f,8.5f),Vector2(18.1f,6.3f),Vector2(16.9f,2.9f));
	
	TankEnemy::shape = new StripShape(93,Vector2(23.7f,-0.1f),Vector2(17.3f,-2.5f),Vector2(12.3f,-4.5f),Vector2(6.9f,-10.7f),Vector2(5.3f,-12.5f),Vector2(3.7f,-11.5f),Vector2(2.5f,-8.1f),Vector2(0.5f,-9.9f),Vector2(-0.1f,-14.1f),Vector2(-3.7f,-16.9f),Vector2(-9.9f,-20.9f),Vector2(-14.7f,-22.9f),Vector2(-17.9f,-22.5f),Vector2(-19.9f,-21.7f),Vector2(-21.9f,-19.9f),Vector2(-22.7f,-15.5f),Vector2(-19.7f,-13.1f),Vector2(-15.3f,-10.5f),Vector2(-14.5f,-8.1f),Vector2(-20.1f,-8.7f),Vector2(-21.5f,-9.3f),Vector2(-22.1f,-9.5f),Vector2(-24.7f,-11.7f),Vector2(-28.3f,-11.3f),Vector2(-27.9f,-5.9f),Vector2(-24.3f,-5.3f),Vector2(-25.9f,-2.5f),Vector2(-14.5f,-7.7f),Vector2(-25.7f,-1.7f),Vector2(-25.5f,1.9f),Vector2(-14.5f,7.9f),Vector2(11.9f,4.7f),Vector2(12.1f,-4.3f),Vector2(2.9f,-5.7f),Vector2(-0.1f,-0.5f),Vector2(3.1f,-5.8f),Vector2(2.7f,-8.0f),Vector2(3.9f,-11.4f),Vector2(6.7f,-11.2f),Vector2(12.5f,-11.0f),Vector2(12.7f,-10.4f),Vector2(15.5f,-10.6f),Vector2(15.5f,-9.2f),Vector2(12.9f,-9.6f),Vector2(12.7f,-9.0f),Vector2(7.9f,-9.4f),Vector2(3.1f,-6.0f),Vector2(3.1f,6.0f),Vector2(7.9f,9.4f),Vector2(12.7f,9.0f),Vector2(12.9f,9.6f),Vector2(15.5f,9.2f),Vector2(15.5f,10.6f),Vector2(12.7f,10.4f),Vector2(12.5f,11.0f),Vector2(6.7f,11.2f),Vector2(3.9f,11.4f),Vector2(2.7f,8.0f),Vector2(3.1f,5.8f),Vector2(-0.1f,0.5f),Vector2(2.9f,5.7f),Vector2(12.1f,4.3f),Vector2(11.9f,-4.7f),Vector2(-14.5f,-7.9f),Vector2(-25.5f,-1.9f),Vector2(-25.7f,1.7f),Vector2(-14.5f,7.7f),Vector2(-25.9f,2.5f),Vector2(-24.3f,5.3f),Vector2(-27.9f,5.9f),Vector2(-28.3f,11.3f),Vector2(-24.7f,11.7f),Vector2(-22.1f,9.5f),Vector2(-21.5f,9.3f),Vector2(-20.1f,8.7f),Vector2(-14.5f,8.1f),Vector2(-15.3f,10.5f),Vector2(-19.7f,13.1f),Vector2(-22.7f,15.5f),Vector2(-21.9f,19.9f),Vector2(-19.9f,21.7f),Vector2(-17.9f,22.5f),Vector2(-14.7f,22.9f),Vector2(-9.9f,20.9f),Vector2(-3.7f,16.9f),Vector2(-0.1f,14.1f),Vector2(0.5f,9.9f),Vector2(2.5f,8.1f),Vector2(3.7f,11.5f),Vector2(5.3f,12.5f),Vector2(6.9f,10.7f),Vector2(12.3f,4.5f),Vector2(17.3f,2.5f));
}

void GeometryWarsController::shutdown()
{
	ASSSERT( GeometryWarsController::wall != nullptr, "Memory clean check" );
	delete GeometryWarsController::wall; 
	delete GeometryWarsController::hourGlass;
	delete GeometryWarsController::bombIndicator;
	
	delete Ship::shipShape;
	delete Ship::turretShape;

	delete Bullet::shape;

	delete Starfield::star;

	delete Secret::jamie;

	delete ChaseEnemy::shape;
	delete TankEnemy::shape;
}

void GeometryWarsController::startGame()
{
	LOG( Severity::INFO, "New Geometry Wars Game has been started" );
	//boss.init();
	reviveTime = 0;
	recursiveRotate = 0;
	enemyCounter = 0;
	respawnTimer = 2.5f;
	lives = 5;
	bombs = 3;
	rot3D = 0;
	bossTimer = 0;
	ship = Ship();
	score = 0;
	parts->reset();
	enemies->reset();
}

void GeometryWarsController::draw( Core::Graphics& graphics )
{
	stars.draw( graphics, globalTransform, ship );
	if(state == MENU)
	{
		graphics.SetColor( RGB( 255, 255, 255 ) );
		graphics.DrawString( SCREENWIDTH/2-64, SCREENHEIGHT/2, "          Ready!\n\nLeft mouse button to start!\n\nPress 'J' to reset life count" );
	}
	
	if(state == GAME)
	{
		STARTPROFILE("Draw");
		graphics.SetColor( RGB( 255, 255, 255 ) );
		
		//Draw game stuff
		drawGame( graphics );

		#ifdef _DEBUG
			drawDebug( graphics );
		#endif

		drawHUD( graphics );

		ENDPROFILE("Draw");
		AutoProfiler::incrementFrame();
	}

	if(state == OVER)
	{
		char buf[200];
		sprintf_s( buf, "                   YOU DIED!\n\n                 High score: %d\n\nPress the left mouse button to go back to the menu", highScore );

		graphics.SetColor( RGB( 255, 255, 255 ) );
		graphics.DrawString( SCREENWIDTH/2-128, SCREENHEIGHT/2, buf );
	}
}

bool GeometryWarsController::update( float dt )
{
	stars.update( dt );

	static bool wasMouseDown = false;
	bool mouseClicked = false;
	bool isMouseDown = Core::Input::IsPressed(Core::Input::BUTTON_LEFT);

	if (!wasMouseDown && isMouseDown)
	{
		mouseClicked = true;
	}

	if(state == MENU)
	{
		if (mouseClicked)
		{
			state = GAME;
			startGame();
		}
	}
	
	if(state == GAME)
	{
		STARTPROFILE("Whole Update");

		upkeep(dt);

		//Jamie button
		if ( Core::Input::IsPressed( 'J' ) )
		{
			lives = 5;
		}

		if (reviveTime < 0)
			input(dt);
		collision(dt);
		enemyHandler(dt);
		updateState(dt);

		ENDPROFILE("Whole Update");
	}

	if(state == OVER)
	{
		if (mouseClicked)
		{
			state = MENU;
			startGame();
		}
	}

	wasMouseDown = isMouseDown;
	return Core::Input::IsPressed( Core::Input::KEY_ESCAPE );
}

void GeometryWarsController::drawDebug( Core::Graphics& graphics )
{
	graphics.SetColor( RGB( 255, 255, 255 ) );
	//Drawing Performance
	char buf[50];
	float time = timer.interval();
	sprintf_s( buf, "%f s", time );
	graphics.DrawString( 100, 100, buf );
	sprintf_s( buf, "%f ms", time*1000 );
	graphics.DrawString( 100, 120, buf );
	sprintf_s( buf, "%f fps", 1/(time) );
	graphics.DrawString( 100, 140, buf );

	//Drawing memory usage
	_CrtMemCheckpoint( &memState );
	sprintf_s( buf, "%d objects", memState.lCounts[_CLIENT_BLOCK] );
	graphics.DrawString( 100, 180, buf );
	sprintf_s( buf, "%d bytes in block", memState.lSizes[_CLIENT_BLOCK] );
	graphics.DrawString( 100, 200, buf );
	sprintf_s( buf, "%d highest", memState.lHighWaterCount );
	graphics.DrawString( 100, 220, buf );

	//Draw Instructions
	graphics.DrawString( 100, 260, "WASD to move" );
	graphics.DrawString( 100, 280, "LMB to shoot and RMB to bomb" );
	graphics.DrawString( 100, 300, "Preprocessor identity LOGGING is defined in GlobalDefinitions.h and log.html/CRASHlog.html are stored in Game" );

	//Debug
	sprintf_s( buf, "%f rot", ship.getRotation() );
	graphics.DrawString( 100, 380, buf );
}

void GeometryWarsController::drawHUD( Core::Graphics& graphics )
{
	graphics.SetColor( RGB( 255, 255, 255 ) );
	for( int i = 1; i <= lives; i++ )
	{
		ship.getShape()->draw( graphics, Matrix3::translation(SCREENWIDTH - i*40.0f,30.0f)*Matrix2::scale(0.5f) );
	}
	for( int i = 1; i <= bombs; i++ )
	{
		bombIndicator->draw( graphics, Matrix3::translation(SCREENWIDTH - i*40.0f,80.0f)*Matrix2::scale(0.5f) );
	}

	char buf[50];
	sprintf_s( buf, "Score: %d", score );
	graphics.DrawString( SCREENWIDTH/2-40, 50, buf );

	float width = 200, height = 30, top = 80;

	graphics.SetColor( RGB( 180, 100, 100 ) );

	float ratio;
	if (boss.getLife() <= 0 )
		ratio = bossTimer/BOSSTIME;
	else
		ratio = boss.getLife()/75.0f;
	if (ratio > 1)
		ratio = 1;
	for( int i = 0; i < height; i++ )
	{
		graphics.DrawLine( SCREENWIDTH/2 - width/2, top+i, SCREENWIDTH/2 - width/2 + int(width * ratio), top+i );
	}
	graphics.SetColor( RGB( 255, 255, 255 ) );
	graphics.DrawLine( SCREENWIDTH/2 - width/2, top, SCREENWIDTH/2 + width/2, top );
	graphics.DrawLine( SCREENWIDTH/2 - width/2, top+height, SCREENWIDTH/2 + width/2, top+height );
	graphics.DrawLine( SCREENWIDTH/2 - width/2, top, SCREENWIDTH/2 - width/2, top+height );
	graphics.DrawLine( SCREENWIDTH/2 + width/2, top, SCREENWIDTH/2 + width/2, top+height );

	if (boss.getLife() > 0 )
		graphics.DrawString( SCREENWIDTH/2 - 75, int(top + 10), "REMAINING BOSS HEALTH" );
	else
		graphics.DrawString( SCREENWIDTH/2 - 75, int(top + 10), "PROGRESS TOWARDS BOSS" );
}

void GeometryWarsController::drawGame( Core::Graphics& graphics )
{
	graphics.SetColor( RGB( 90, 90, 90 ) );
	STARTPROFILE("Recursive Background");
	recursiveSpin( graphics, 6, globalTransform * Matrix3::translation( SCREENWIDTH/(2.0f) ,SCREENHEIGHT/(2.0f) ), 80, RGB( 80,150,80 ) );
	ENDPROFILE("Recursive Background");
	graphics.SetColor( RGB( 255, 255, 255 ) );

	STARTPROFILE("Particle Draw");
	parts->draw( graphics, globalTransform );
	ENDPROFILE("Particle Draw");

	if (reviveTime < 0)
		ship.draw( graphics, globalTransform );

	boss.draw( graphics, globalTransform );

	wall->drawGlow( graphics, globalTransform, RGB(0,0,255) );

	enemies->draw( graphics, globalTransform );

	STARTPROFILE("Bullet Draw");
	for ( auto &i : bullets ) 
	{
		i.draw(graphics, globalTransform );
	}
	ENDPROFILE("Bullet Draw");
}

void GeometryWarsController::upkeep( float dt )
{
	reviveTime -= dt;
	bossTimer += dt;
	if( bossTimer > BOSSTIME && boss.getLife() <= 0 )
	{
		boss.init();
	}
	rot3D -= dt/2;
	if (rot3D < 0)
		rot3D = 0;
	recursiveRotate += dt;
	if (reviveTime < 0)
	{
		enemyCounter += dt;
		if (boss.getLife() > 0)
		{
			enemyCounter -= dt/2;
		}
		score += int(dt*100);
	}

	//Set global transform for exploding and such
	globalTransform = Matrix4::translation( SCREENWIDTH/2, SCREENHEIGHT/2 ) * /*Matrix4::rotation( axis, rot3D )*/Matrix4::rotation( MathHelper::randomUnitVector(), (MathHelper::randomFloat()*2-1)*rot3D ) * Matrix4::translation( -SCREENWIDTH/2, -SCREENHEIGHT/2 );
}

void GeometryWarsController::input( float dt )
{
	bool isMouseDown = Core::Input::IsPressed(Core::Input::BUTTON_LEFT);
	Matrix4 globalInv = globalTransform.inverse();

	//Set turret
	Vector2 mouse = globalInv * Vector2((float)Core::Input::GetMouseX(),(float)Core::Input::GetMouseY());
	ship.setTurret( mouse-ship.getPosition() );

	//Accelerating
	int right	= Core::Input::IsPressed( 'D' );
	int left	= Core::Input::IsPressed( 'A' );
	int up		= Core::Input::IsPressed( 'W' );
	int down	= Core::Input::IsPressed( 'S' );
	ship.setAcceleration( ((right - left)*Vector2(1,0) + (up - down)*Vector2(0,-1) ) * Ship::SPEED  );

	//Streamer
	if( (right - left) != 0 || (up - down) != 0 )
	{
		ParticleEffectHelper::emitStream( parts, RGB(0,255,255), ship.getPosition()+ MathHelper::randomUnitVector()*5, -ship.getAcceleration(), 200 , dt*100 );
	}

	//Shooting
	if (isMouseDown&&ship.canShoot())
	{
		bullets.push_back( ship.shoot() );
	}

	//Bombing
	if ( mouseWasDown == false && Core::Input::IsPressed( Core::Input::BUTTON_RIGHT ) && bombs > 0 )
	{
		ParticleEffectHelper::createExplosion( parts, ship.getPosition(), 1500, 1000 );
		bombs -= 1;
		mouseWasDown = true;
		Enemy ** enem = enemies->getContained();
		for( int i = 0; i < EnemyBatch::ENEMY_LIMIT; i++ )
		{
			if( (*(enem+i))->isAlive() )
			{
				(*(enem+i))->kill();
				score+=1000;
				ParticleEffectHelper::createExplosion( parts, (*(enem+i))->getPosition(), 100, 64 );
			}
		}
	}
	if ( !Core::Input::IsPressed( Core::Input::BUTTON_RIGHT ) )
	{
		mouseWasDown = false;
	}
}

void GeometryWarsController::collision( float dt )
{
	//phase3
	if (Core::Input::IsPressed( '3' ))
	{
		if (currentCollision != 3)
		{
			ship.setPosition( Vector2( 500, 500 ) );
		}
		currentCollision = 3;
	}
	if (currentCollision == 3)
	{
		if ( wall->crossed( *ship.getShape(), ship.getPosition(), ship.getPosition()+ship.getVelocity()*dt ) )
		{
			ship.setVelocity( ship.getVelocity().reflect( wall->closestNormal( *ship.getShape(), ship.getPosition() ) ) );
			ship.setAcceleration( Vector2() );
			ship.goToPrevious();
		}
	}
}

void GeometryWarsController::enemyHandler( float dt )
{
	//Boss
	boss.update( dt, ship, bullets );
	if ( boss.getLife() > 0 )
	{
		if ((ship.getPosition()-boss.getPosition()).lengthSquared() < 190*190 || boss.collide(ship))
		{
			death();
			bullets.clear();
		}
	}

	//Create Enemy
	if ( enemyCounter > respawnTimer )
	{
		//respawnTimer -= 0.1f;
		if (respawnTimer < 0.5f )
			respawnTimer = 0.5f;
		Enemy* en = enemies->createEnemy();
		en->setPosition( Vector2( (float)MathHelper::randomInt( SCREENWIDTH/8, SCREENWIDTH*7/8 ), (float)MathHelper::randomInt( SCREENHEIGHT/8, SCREENHEIGHT*7/8 ) ) );
		enemyCounter = 0;
	}

	//Loop through enemies
	Enemy ** enem = enemies->getContained();
	for( int i = 0; i < EnemyBatch::ENEMY_LIMIT; i++ )
	{
		Enemy * currentEnem = *(enem+i);
		(std::cout << currentEnem->isAlive());
		if( currentEnem->isAlive() )
		{
			//Collision
			if ( wall->crossed( *currentEnem->getShape(), currentEnem->getPosition(), currentEnem->getPosition()+currentEnem->getVelocity()*dt ) )
			{
				currentEnem->setVelocity( currentEnem->getVelocity().reflect( wall->closestNormal( *currentEnem->getShape(), currentEnem->getPosition() ) ) );
			}

			//Pass control to Enemy to determine its stepwise behavior
			currentEnem->behave( dt, ship, bullets );

			//Streamer
			ParticleEffectHelper::emitStream( parts, RGB(255,0,255), currentEnem->getPosition() + MathHelper::randomUnitVector()*5, -currentEnem->getAcceleration(), 180 , dt*80 );

			//Colliding with player
			if( (currentEnem->getPosition()-ship.getPosition()).lengthSquared() < 32*32 )
			{
				currentEnem->kill();
				ParticleEffectHelper::createExplosion( parts, currentEnem->getPosition(), 100, 64 );
				death();

				bullets.clear();
			}
		}
	}
}

void GeometryWarsController::updateState( float dt )
{
	STARTPROFILE("Object sans Bullet");
	ship.update(dt);
	enemies->update(dt);
	parts->update(dt);
	ENDPROFILE("Object sans Bullet");
	bulletUpdate(dt);
}

void GeometryWarsController::bulletUpdate( float dt )
{
	STARTPROFILE("Bullet Update");
	Enemy ** enem = enemies->getContained();
	bool bulletsCleared = false;
	//Loop through bullets
	for(std::vector<Bullet>::iterator it = bullets.begin(); it != bullets.end(); )
	{
		it->update(dt);
		
		ParticleEffectHelper::emitStream( parts, RGB(190,140,140), it->getPosition() + MathHelper::randomUnitVector()*2, -it->getVelocity(), 200 , dt*70 );

		Bullet bull = *it;
		bool destroyed = false;

		//Colliding with Enemies
		if ( bull.isFromPlayer() )
		{
			//boss
			if ( (bull.getPosition()-boss.getPosition()).lengthSquared() < 190*190 && boss.getLife() > 0 )
			{
				it = bullets.erase( it );
				boss.hit();
				ParticleEffectHelper::createExplosion( parts, bull.getPosition(), 100, 60 );
				destroyed = true;
				if( boss.getLife() <= 0 )
				{
					ParticleEffectHelper::createExplosion( parts, boss.getPosition(), 1500, 1000 );
					bossTimer = 0;
					score += 20000;
				}
			}
			else
			for( int i = 0; i < EnemyBatch::ENEMY_LIMIT; i++ )
			{
				Enemy * currentEnem = (*(enem+i));
				if( !destroyed && currentEnem->isAlive() )
				{
					float distSquare = ( bull.getPosition() - currentEnem->getPosition() ).lengthSquared();
					if (distSquare < 32*32)
					{
						it = bullets.erase( it );
						score += 1000;
						currentEnem->kill();
						ParticleEffectHelper::createExplosion( parts, currentEnem->getPosition(), 150, 96 );
						destroyed = true;
					}
				}
			}
		}
		else
		//Colliding with player
		{
			float distSquare = ( bull.getPosition() - ship.getPosition() ).lengthSquared();
			if (distSquare < 32*32)
			{
				it = bullets.erase( it );
				death();
				
				ParticleEffectHelper::createExplosion( parts, bull.getPosition(), 150, 96 );
				destroyed = true;

				
				bulletsCleared=true;
			}
		}

		//Colliding with walls
		if ( !destroyed && wall->crossed( *bull.getShape(), bull.getPosition() - bull.getVelocity()*dt, bull.getPosition() ) )
		{
			it = bullets.erase( it );
			ParticleEffectHelper::createExplosion( parts, bull.getPosition(), 50, 32 );
			destroyed = true;
		}
		if( !destroyed )
			it++;
	}

	if (bulletsCleared)
	{
		bullets.clear();
	}
	ENDPROFILE("Bullet Update");
}

void GeometryWarsController::recursiveSpin( Core::Graphics& graphics, int numRecurse, Matrix4 total, float scale, Core::RGB color )
{
	float translateAmount = 4;
	
	if ( numRecurse > 0 )
	{
		recursiveSpin( graphics, numRecurse - 1, total * Matrix3::rotation( recursiveRotate ) * Matrix3::translation( translateAmount*scale, 0 ), scale * 0.6f, ColorHelper::lerp( color, RGB(0,0,0), 0.2f ) );
		//recursiveSpin( graphics, numRecurse - 1, total * Matrix3::rotation( recursiveRotate + PI/2 ) * Matrix3::translation( translateAmount*scale, 0 ), scale * 0.6f );
		recursiveSpin( graphics, numRecurse - 1, total * Matrix3::rotation( recursiveRotate + PI ) * Matrix3::translation( translateAmount*scale, 0 ), scale * 0.6f, ColorHelper::lerp( color, RGB(0,0,0), 0.2f ) );
		//recursiveSpin( graphics, numRecurse - 1, total * Matrix3::rotation( recursiveRotate + 3*PI/2 ) * Matrix3::translation( translateAmount*scale, 0 ), scale * 0.6f );
	}

	graphics.SetColor(color);
	hourGlass->draw( graphics, total * Matrix3::scale( scale ) );
}

void GeometryWarsController::death()
{
	boss.wait();
	lives--;
	rot3D = 0.7f;

	if ( lives < 0 )
	{
		boss.disable();
		highScore = (score > highScore ) ? score : highScore ;
		state = OVER;
	}

	ParticleEffectHelper::createExplosion( parts, ship.getPosition(), 1500, 1000 );
	Enemy ** enem = enemies->getContained();
	for( int i = 0; i < EnemyBatch::ENEMY_LIMIT; i++ )
	{
		if( (*(enem+i))->isAlive() )
		{
			(*(enem+i))->kill();
			ParticleEffectHelper::createExplosion( parts, (*(enem+i))->getPosition(), 100, 64 );
		}
	}
	ship = Ship();
	reviveTime = 2;

	if( boss.getLife() > 0 )
	{
		ship.setPosition( ship.getPosition() + Vector2( 0, 300 ) );
	}
}