#pragma once

#include "definition.h"

struct Kerning;
struct Character;
struct Texture;

struct EXPORT Font
{
	byte lineHeight, baseHeight;
	unsigned short width, height;
	byte minChar, maxChar, chars, kerns;

	byte *characterPositions;
	Character *characters;
	Kerning *kernings;

	Texture *texture;

	~Font();

	Character* getCharacter( const char character ) const;
	byte getKerningAmount( const char character1, const char character2 ) const;
};