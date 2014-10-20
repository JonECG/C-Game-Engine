#include "Engine.h"
#include "PubHandle.h"
#include <crtdbg.h>

void main( int argc, char* argv[] )
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );

	Engine::init( argc, argv );

	int * test = new int[30];
	test[5] = 3;
	delete [] test;

	PubHandle* pub = new PubHandle();
	Engine::run( pub );
	Engine::shutdown();
	//delete pub;
}