#include "Renderable.h"

#include "Geometry.h"
#include "Shader.h"
#include "Texture.h"

#include "ParameterType.h"
#include "GL\glew.h"

const int MAX_RENDERABLES = 10000;
Renderable rends[MAX_RENDERABLES];
int currentRenderable = 0;
Renderable * Renderable::nextFreeRenderable()
{
	return &rends[currentRenderable++];
}

Renderable::Renderable()
{
	numUniformParameters = 0;
}

Renderable * Renderable::makeCopy() const
{
	return whatGeometry->makeRenderable( howShader, texture, true );
}

void Renderable::sendUniforms() const
{
	howShader->sendUniforms();

	for( unsigned int i = 0; i < numUniformParameters; i++ )
	{
		uniformParameters[i].send();
	}
}

void Renderable::addUniformParameter( const char* name, ParameterType parameterType, const void* value)
{
	GLint loc = glGetUniformLocation(howShader->shaderProgramID, name);
	uniformParameters[ numUniformParameters ].location = loc;
	uniformParameters[ numUniformParameters ].data = value;
	uniformParameters[ numUniformParameters ].type = parameterType;
	numUniformParameters++;
}

void Renderable::setUniformParameter( const char* name, ParameterType parameterType, const void* value)
{
	howShader->setUniformParameter( name, parameterType, value );
}