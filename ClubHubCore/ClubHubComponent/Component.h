#pragma once
#include "definition.h"
#include "glmfwd.h"

class Entity;
class EXPORT Component
{	
	int id;
	bool inited;
protected:
	Entity * parent;
public:
	Component();
	Entity * getParent();
	virtual ~Component();
	virtual int getId();
	virtual void init();
	virtual void destroy();
	virtual void draw();
	virtual void update( float dt );
	virtual void lateUpdate( float dt );
	virtual void earlyUpdate( float dt );
	virtual void onCollide( Entity * other, glm::vec3 collisionNormal, float interpenetration );
	friend Entity;
	friend class Stage;
};

