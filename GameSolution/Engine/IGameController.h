#ifndef IGAMECONTROLLER_H
#define IGAMECONTROLLER_H

#include "Core.h"

class IGameController
{
public:
	inline virtual ~IGameController(){};
	virtual void init() = 0;
	virtual void shutdown() = 0;
	virtual bool update( float dt ) = 0;
	virtual void draw( Core::Graphics& graphics ) = 0;
};

#endif