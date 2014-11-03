#include "Geometry.h"
#include <GL\glew.h>
#include "ParameterType.h"
#include "Renderable.h"

void Geometry::addAttribute( unsigned int layoutLocation, ParameterType parameterType, unsigned int bufferOffset, unsigned int bufferStride)
{
	glBindVertexArray( vertexArrayID );
	glEnableVertexAttribArray( layoutLocation );
	glVertexAttribPointer( layoutLocation, parameterType / sizeof(float), GL_FLOAT, GL_FALSE, bufferStride, (void*)( bufferOffset + this->bufferOffset + numIndices*sizeof(GLushort) )  );
}

Renderable* Geometry::makeRenderable( Shader* shader, const Texture* texture, bool visible )
{
	Renderable* result = Renderable::nextFreeRenderable();

	result->whatGeometry = this;
	result->howShader = shader;
	result->texture = texture;
	result->visible = visible;

	return result;
}