#pragma once
#include "Component.h"

class BulletComponent : public Component
{
public:
	float speed;
	BulletComponent();
	void update( float dt );
};

