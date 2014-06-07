#include <iostream>
#include "Engine.h"
#include "GeometryWarsController.h"
#include "AutoProfiler.h"
#include "Log.h"
#include "DebugMemory.h"
#include "assert.h"

int main()
{
	LOG( Severity::INFO, "Program Entry" );
	//Lab - Memory Management
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF );


	//Test Memory Leak
	/*
	char* leak = new char[500];
	leak[20] = 's';
	std::cout << leak[20];
	*/

	//Test Corrupted Memory
	/*
	char * bad = new char[2];
	bad[4] = '4';
	delete bad;
	*/

	//Lab - Logging
	LOG( Severity::EROR, "Showcase Error" );
	LOG( Severity::INFO, "Showcase Info" );
	LOG( Severity::WARNING, "Showcase Warning" );
	LOG( Severity::SEVERE, "Showcase Severe" );

	//TODO: Uncomment to test assertion failure
	/*for( int i = 0; i < 3; i++ )
	{
		ASSERT( i == 0, "Showcase Assert" );
	}*/

	Engine::Init( new GeometryWarsController() );
	Engine::Run();
	Engine::Shutdown();

	//Lab - Logging
	LOG( Severity::INFO, "Program End" );
	Logger::outputLog( "log.html" );
}

