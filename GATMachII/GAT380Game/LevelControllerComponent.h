#pragma once
#include "Component.h"
struct Renderable;
class LevelControllerComponent : public Component
{
	Entity * player;
	float time;
	void startGame();
public:
	Renderable *friendlyPlane, *enemyPlane, *ground, *bullet;

	LevelControllerComponent();
	void init();
	void update( float dt );
};

