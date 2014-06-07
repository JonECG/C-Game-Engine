#ifndef GEOMETRYWARSCONTROLLER_H
#define GEOMETRYWARSCONTROLLER_H

#include "Ship.h"
#include "Vector2.h"
#include <Vector>
#include "IGameController.h"
#include "ParticleEffect.h"
#include "AutoProfiler.h"
#include "EnemyBatch.h"
#include "MathHelper.h"
#include "DebugMemory.h"
#include "Log.h"
#include "Starfield.h"
#include "Secret.h"

using std::vector;

class GeometryWarsController : public IGameController
{
	Ship ship;

	static const float PI;
	int currentCollision;

	static const int SCREENWIDTH = 1280, SCREENHEIGHT = 960;

	static Shape *wall, *hourGlass, *bombIndicator;
	float respawnTimer;
	float recursiveRotate;
	float enemyCounter;
	float reviveTime;

	Starfield stars;

	bool mouseWasDown;
	bool trackingMouse;

	_CrtMemState memState;

	vector<Bullet> bullets;

	ParticleEffect * parts;
	EnemyBatch * enemies;

	Timer timer;

	static const int MENU = 0, GAME = 1, OVER = 2;
	static const int BOSSTIME = 20;
	int state;
	float bossTimer;
	int lives;
	int bombs;
	int highScore;
	int score;

	Secret boss;

	Vector3 axis;
	float rot3D;

	Matrix4 globalTransform;
	
	void drawGame( Core::Graphics& graphics );
	void drawDebug( Core::Graphics& graphics );
	void drawHUD( Core::Graphics& graphics );

	void upkeep( float dt );
	void input( float dt );
	void collision( float dt );
	void enemyHandler( float dt );
	void updateState( float dt );
	void bulletUpdate( float dt );

	void startGame();
	void recursiveSpin( Core::Graphics& graphics, int numRecurse, Matrix4 total, float scale, Core::RGB color );
public:
	GeometryWarsController();
	~GeometryWarsController();

	bool update( float dt );
	void draw( Core::Graphics& g );

	void init();
	void shutdown();
	void death();
};

#endif