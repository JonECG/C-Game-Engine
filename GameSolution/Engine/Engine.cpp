#include "Engine.h"
#include <stdlib.h>
#include <time.h>
#include "AutoProfiler.h"

namespace Engine
{
	IGameController * currentGameController = NULL;

	void Init( IGameController * controller )
	{
		LOG( Severity::INFO, "Attaching Game Controller" );
		srand((int)time(NULL));
		Shutdown();
		currentGameController = controller;
		controller->init();
		AutoProfiler::init();
		Core::RegisterDrawFn( draw );
		Core::RegisterUpdateFn( update );
	}

	void Run()
	{
		Core::GameLoop();
	}

	bool update( float dt )
	{
		return currentGameController->update( dt );
	}

	void draw( Core::Graphics& g )
	{
		currentGameController->draw( g );
	}

	bool sampleFunctionThatReturnsTrue()
	{
		return false;
	}

	void Shutdown()
	{
		if (currentGameController != NULL)
		{
			LOG( Severity::INFO, "Shutting Down Game Controller" );
			currentGameController->shutdown();
			delete currentGameController;
			currentGameController = NULL;
		}
		Core::Shutdown();
		LOG( Severity::INFO, "Refreshing Profiles" );
		AutoProfiler::writeOutput( "totalProfiling.csv", "frameProfiling.csv" );
	}
}
