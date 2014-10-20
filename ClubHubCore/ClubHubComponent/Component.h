#pragma once
#include "definition.h"
#include "glmfwd.h"

class Entity;
class EXPORT Component
{	
	int id;
protected:
	Entity * parent;
public:
	Component();
	virtual ~Component();
	virtual int getId();
	virtual void init();
	virtual void destroy();
	virtual void draw();
	virtual void update( float dt );
	virtual void onCollide( Entity * other, glm::vec3 collisionNormal, float interpenetration );
	friend Entity;
};

