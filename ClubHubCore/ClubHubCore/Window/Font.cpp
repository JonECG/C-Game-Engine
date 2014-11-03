#include "Font.h"
#include "Character.h"
#include "Kerning.h"

Character* Font::getCharacter( const char character ) const
{
	return &characters[character-minChar];
}

Font::~Font()
{
	delete [] characterPositions;
	delete [] characters;
	delete [] kernings;
}

byte Font::getKerningAmount( const char character1, const char character2 ) const
{
	character1;
	character2;
	return 0;
}