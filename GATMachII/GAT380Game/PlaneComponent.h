#pragma once
#include "Component.h"
struct Renderable;
class PlaneComponent : public Component
{
	float health;
	float coolDown;
public:
	int alignment;
	Renderable * bullet;
	float dir;
	float speed;
	float maxSpeed;

	void fire();

	PlaneComponent();
	void init();
	void destroy();
	void draw();
	void update( float dt );
	void onCollide( Entity * other, glm::vec3 collisionNormal, float interpenetration );
};

