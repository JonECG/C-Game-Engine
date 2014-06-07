#pragma once

#include <windows.h>
#include <GL\glew.h>

class Timer
{
	LARGE_INTEGER startInt, stopInt, lastIntervalInt, frequencyInt;
	GLfloat LIToSecs( LARGE_INTEGER & l );
	GLfloat getDifference( LARGE_INTEGER& prev, LARGE_INTEGER& next );
public:
	Timer();
	void start();
	GLfloat stop();
	GLfloat interval();
};

