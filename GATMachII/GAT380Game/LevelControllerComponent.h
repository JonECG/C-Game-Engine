#pragma once
#include "Component.h"
struct Renderable;
class LevelControllerComponent : public Component
{
	Entity * player;
	Entity * planet;

	Entity * enemies[30];
	int numEnemies;

	bool canRestart;
	const char * restartMessage;

	float time;
	void startGame();
public:
	Renderable *friendlyPlane, *enemyPlane, *ground, *bullet, *indicator;

	LevelControllerComponent();
	void init();
	void update( float dt );
	void draw();
};

