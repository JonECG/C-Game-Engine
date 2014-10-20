#include "Engine.h"
#include "PubHandle.h"

void main( int argc, char* argv[] )
{
	Engine::init( argc, argv );

	PubHandle* pub = new PubHandle();
	Engine::run( pub );
	delete pub;

	Engine::shutdown();
}