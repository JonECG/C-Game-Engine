#include "Log.h"

#ifdef LOGGING
Severity Logger::severities[ENTRIES];
char Logger::messages[ENTRIES][200];
int Logger::lines[ENTRIES];
char Logger::files[ENTRIES][100];
int Logger::currentEntry = 0;

void Logger::log( Severity s, char * file, int line, char* message )
{
	severities[currentEntry] = s;
	strcpy_s( messages[currentEntry], message );
	lines[currentEntry] = line;
	strcpy_s( files[currentEntry], file );
	currentEntry++;
}

void Logger::outputLog( const char* fileName )
{
	std::ofstream myFile;
	myFile.open(fileName);

	myFile << "<body bgcolor=\"#CCCCCC\">" << std::endl;
	for( int i = 0; i < currentEntry; i++ )
	{
		char * color;
		switch( severities[i] )
		{
		case Severity::INFO:
			color = "000000";
			break;
		case Severity::WARNING:
			color = "FFFF00";
			break;
		case Severity::EROR:
			color = "FF0000";
			break;
		case Severity::SEVERE:
			color = "CC0000";
			break;
		default:
			color = "0000FF";
			break;
		}

		myFile << "<p style=\"color: #" << color << "\">";

		myFile << files[i] << "(" << lines[i] << "):" << messages[i];
	
		myFile << "</p>" << std::endl;
	}
	myFile << "</body>" << std::endl;

	myFile.close();
}
#else
void Logger::log( Severity s, char * file, int line, char* message ){s;file;line;message;}
void Logger::outputLog( const char* fileName ){fileName;}
#endif