#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "ExportHeader.h"

//#include "Log.h"

namespace Engine
{
	// your stuff goes here...
	// use ENGINE_SHARED to indicate a function or class that you want to export
	ENGINE_SHARED void Init();

	ENGINE_SHARED void Run();

	ENGINE_SHARED void Shutdown();
	
}

#endif // _ENGINE_H_