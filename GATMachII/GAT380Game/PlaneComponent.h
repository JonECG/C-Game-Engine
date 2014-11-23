#pragma once
#include "Component.h"
#include <glm\glm.hpp>
struct Renderable;
class PlaneComponent : public Component
{
	float health;
	float coolDown;
public:
	int alignment;
	Renderable * bullet;
	float height;
	float turning;
	glm::vec3 dir;
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

