#pragma once
#include "definition.h"

class EXPORT Buffer
{
public:
	static const unsigned int SIZE = 1000000;
	unsigned int freeptr;
	unsigned int id;

	Buffer();
};

