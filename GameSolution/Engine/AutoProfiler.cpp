#include "AutoProfiler.h"
#include <time.h>

int AutoProfiler::currentEntry = 0;
int AutoProfiler::currentFrame = 0;
long AutoProfiler::epoch = 0;
long AutoProfiler::frameTime = 0;

int AutoProfiler::timesProfiled[ENTRIES];
float AutoProfiler::totalTime[ENTRIES];
char AutoProfiler::namesProfiled[ENTRIES][40];
int AutoProfiler::framesProfiled = 0;

long AutoProfiler::epochProfiled[FRAMES_HELD];
float AutoProfiler::instanceTimeProfiled[ENTRIES][FRAMES_HELD];
Timer AutoProfiler::frameTimer = Timer();

AutoProfiler::AutoProfiler(const char* name)
{
	this->name = name;
	timer.start();
}


AutoProfiler::~AutoProfiler()
{
	log( name, timer.stop()*1000 );
}

#ifdef PROFILING
void AutoProfiler::init()
{
	epoch = 0;
	frameTimer.interval();
	currentFrame = 0;
	framesProfiled = 0;
}

void AutoProfiler::incrementFrame()
{
	epoch += (long)(frameTimer.interval()*1000);
	epochProfiled[currentFrame] = epoch;
	currentFrame = (currentFrame + 1) % FRAMES_HELD;
	framesProfiled++;
	for( int i = 0; i < ENTRIES; i++ )
	{
		instanceTimeProfiled[i][currentFrame] = 0;
	}
}

void AutoProfiler::updateEntry( int index, float timed )
{
	totalTime[index] += timed;
	timesProfiled[index]++;
	instanceTimeProfiled[index][currentFrame] += timed;
}

void AutoProfiler::log( const char * name, float timed )
{
	bool found = false;
	for( int i = 0; i < currentEntry; i++ )
	{
		if ( strcmp( namesProfiled[i], name ) == 0 )
		{
			updateEntry( i, timed );
			found = true;
		}
	}

	if (!found)
	{
		strcpy_s( namesProfiled[currentEntry], name );
		updateEntry( currentEntry, timed );
		//totalTime[currentEntry] += timed;
		//timesProfiled[currentEntry]++;
		currentEntry++;
	}
}

void AutoProfiler::writeOutput( const char * totalFileName, const char * frameFileName )
{
	std::ofstream totFile;
	totFile.open(totalFileName);

	totFile << "Profiling Segment, Times Called, Total Time(ms), Internal Avg(ms)" << std::endl;
	for( int i = 0; i < currentEntry; i++ )
	{
		totFile << namesProfiled[i] << ", " << timesProfiled[i] << ", " << totalTime[i] << ", " << (totalTime[i]/timesProfiled[i]) << std::endl;
	}
	totFile.close();

	std::ofstream frameFile;
	frameFile.open(frameFileName);

	int howMany = (framesProfiled < FRAMES_HELD) ? framesProfiled : FRAMES_HELD;

	frameFile << "Relative Frame No.,Time (ms)";
	for( int i = 0; i < currentEntry; i++ )
	{
		frameFile << "," << namesProfiled[i];
	}
	frameFile << std::endl;
	int initialFrame = FRAMES_HELD-(howMany);
	for( int i = initialFrame; i < FRAMES_HELD; i++ )
	{
		int index = ( i + currentFrame + FRAMES_HELD ) % FRAMES_HELD;
		frameFile << i-initialFrame << "," << epochProfiled[index];
		for( int j = 0; j < currentEntry; j++ )
		{
			frameFile << "," << instanceTimeProfiled[j][index];
		}
		frameFile << std::endl;
	}

	frameFile << framesProfiled << " frames profiled -- showing last " << howMany << std::endl;

	frameFile.close();
}
#else
void AutoProfiler::init(){}

void AutoProfiler::incrementFrame(){}

void AutoProfiler::updateEntry( int index, float timed ){index;timed;}

void AutoProfiler::log( const char * name, float timed ){name;timed;}

void AutoProfiler::writeOutput( const char * totalFileName, const char * frameFileName ){totalFileName;frameFileName;}
#endif