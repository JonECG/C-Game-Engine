#include "Timer.h"

Timer::Timer(void)
{
	QueryPerformanceFrequency( &frequencyInt );
	QueryPerformanceCounter(&startInt);
	QueryPerformanceCounter(&lastIntervalInt);
}

GLfloat Timer::LIToSecs( LARGE_INTEGER & l )
{
	return ((GLfloat)l.QuadPart / (GLfloat)frequencyInt.QuadPart);
}

void Timer::start()
{
	QueryPerformanceCounter(&startInt);
}

GLfloat Timer::stop() 
{
	QueryPerformanceCounter(&stopInt);
	return getDifference( startInt, stopInt );
}

GLfloat Timer::interval()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	GLfloat result = getDifference( lastIntervalInt, currentTime );
	lastIntervalInt = currentTime;
	return result;
}

GLfloat Timer::getDifference( LARGE_INTEGER& prev, LARGE_INTEGER& next )
{
	LARGE_INTEGER time;
	time.QuadPart = next.QuadPart - prev.QuadPart;
	return LIToSecs( time ) ;
}