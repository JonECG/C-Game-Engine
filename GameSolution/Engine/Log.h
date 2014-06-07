#ifndef LOG_H
#define LOG_H

#include <ostream>
#include <fstream>
#include "ExportHeader.h"
#include "GlobalDefinitions.h"

enum Severity{ INFO, WARNING, EROR, SEVERE };

class ENGINE_SHARED Logger
{

#ifdef LOGGING
	static const int ENTRIES = 100;
	static Severity severities[ENTRIES];
	static char messages[ENTRIES][200];
	static int lines[ENTRIES];
	static char files[ENTRIES][100];
	static int currentEntry;
#endif

public:
	static void log( Severity s, char * file, int line, char* message );
	static void outputLog( const char* fileName );
};

#ifdef LOGGING
#define LOG( a, b ) std::cout << __FILE__ << "(" << __LINE__ << "): " << b << std::endl; Logger::log( a, __FILE__, __LINE__ ,b )
#include "assert.h"
#else
#define LOG( a, b ) Logger::log( a, __FILE__, __LINE__ ,b )
#endif

#endif