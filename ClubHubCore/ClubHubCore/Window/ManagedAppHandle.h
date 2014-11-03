#pragma once

///////////////////////////////////////////////////////////////////////////////
///////////				To create your own app handle:				///////////
/*///////////////////////////////////////////////////////////////////////////// 

#pragma once

#include "ManagedAppHandle.h"

class MyDerivedHandle : public ManagedAppHandle
{
public:
	void init( ContentManager *content, CoordinateTransform *transform );
	void paint( Graphics *g );
	void update( float dt, const UserController *uc );
};

*////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#include "definition.h"
class ContentManager;
class Graphics;
class UserController;
class CoordinateTransform;

class ManagedAppHandle
{
protected:
	void EXPORT debug( const char* message );
	void EXPORT debug( long message );
	void EXPORT debug( double message );
public:
	virtual void init( ContentManager *content, CoordinateTransform *transform, Graphics *g ) = 0;
	virtual void paint( Graphics *g ) = 0;
	virtual void shutdown() = 0;
	virtual void update( float dt, const UserController *uc ) = 0;
};