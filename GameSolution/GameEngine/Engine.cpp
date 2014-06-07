#include "Engine.h"
#include <stdlib.h>
#include <time.h>
//#include "AutoProfiler.h"

namespace Engine
{
	void Init()
	{
		//LOG( Severity::INFO, "Attaching Game Controller" );
		srand((int)time(NULL));
		//AutoProfiler::init();
	}


	void Shutdown()
	{
		//LOG( Severity::INFO, "Refreshing Profiles" );
		//AutoProfiler::writeOutput( "totalProfiling.csv", "frameProfiling.csv" );
	}
}
