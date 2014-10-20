#include "Character.h"

unsigned short Character::getX() const
{
	return *(reinterpret_cast<const short*>(&x));
}

unsigned short Character::getY() const
{
	return *(reinterpret_cast<const short*>(&y));
}