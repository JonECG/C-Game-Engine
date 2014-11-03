#include "Buffer.h"
#include "GL\glew.h"

Buffer::Buffer()
{
	freeptr = 0;
	id = 0;

	glGenBuffers(1,&id);

	glBindBuffer( GL_ARRAY_BUFFER, id );
	glBufferData( GL_ARRAY_BUFFER, SIZE, 0, GL_STATIC_DRAW );
}
