#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "ExportHeader.h"

#include "Core.h"

#include "GlobalDefinitions.h"

#include "Log.h"

#include "IGameController.h"

namespace Engine
{
	// your stuff goes here...
	// use ENGINE_SHARED to indicate a function or class that you want to export
	ENGINE_SHARED void Init( IGameController * controller );

	ENGINE_SHARED void Run();

	ENGINE_SHARED bool update( float dt );

	ENGINE_SHARED void draw( Core::Graphics& g );

	ENGINE_SHARED bool sampleFunctionThatReturnsTrue();

	ENGINE_SHARED void Shutdown();
	
}

#endif // _ENGINE_H_