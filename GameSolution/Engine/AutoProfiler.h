#ifndef AUTOPROFILER_H
#define AUTOPROFILER_H

#include "Timer.h"
#include <fstream>
#include <iostream>
#include "ExportHeader.h"
#include "GlobalDefinitions.h"

class ENGINE_SHARED AutoProfiler
{
	static const int ENTRIES = 100;
	static const int FRAMES_HELD = 400;
	static int currentEntry;
	static long epoch;
	static long frameTime;

	//Summary
	static int timesProfiled[ENTRIES];
	static float totalTime[ENTRIES];
	static char namesProfiled[ENTRIES][40];
	
	//Per call
	static int currentFrame;//[ENTRIES];
	static int framesProfiled;
	static long epochProfiled[FRAMES_HELD];
	static float instanceTimeProfiled[ENTRIES][FRAMES_HELD];

	static void updateEntry( int index, float time );

	const char * name;
	Timer timer;
	static Timer frameTimer;
public:
	AutoProfiler(const char * name);
	~AutoProfiler();
	static void init();
	static void log( const char * name, float time );
	static void writeOutput( const char * totalFileName, const char * frameFileName );
	static void incrementFrame();
};

#ifdef PROFILING
#define STARTPROFILE(name) { AutoProfiler p(name)
#else
#define STARTPROFILE(name) {
#endif

#define ENDPROFILE(name) }

#endif