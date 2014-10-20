#pragma once

#include "definition.h"

struct EXPORT Character
{
	byte x, xb, y, yb, width, height, xoff, yoff, advance;

	unsigned short getX() const;
	unsigned short getY() const;
};