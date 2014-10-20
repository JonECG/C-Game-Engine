#pragma once
#include "Component.h"
struct Renderable;
class PlaneComponent : public Component
{
	float health;

public:
	Renderable * bullet;
	float dir;
	float speed;

	void fire();

	void init();
	void destroy();
	void draw();
	void update( float dt );
	void onCollide( Entity * other, glm::vec3 collisionNormal, float interpenetration );
};

