#ifndef IORGANIC_H
#define IORGANIC_H

#include "Core.h"
#include "Matrix4.h"

class ENGINE_SHARED IOrganic
{
public:
	inline virtual ~IOrganic(){};
	virtual bool isAlive() const = 0;
	virtual void kill() = 0;
	virtual void draw( Core::Graphics& g ) const = 0;
	virtual void draw( Core::Graphics& g, Matrix4& globalTransform  ) const = 0;
	virtual void update( float dt ) = 0;
};

#endif