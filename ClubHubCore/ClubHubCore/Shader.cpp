#include "Shader.h"

#include <GL\glew.h>
#include "ParameterType.h"

unsigned int numShadersAssigned = 0;
Shader assignedShaders[20];

unsigned int numGlobalUniformParameters = 0;
UniformInfo globalUniformParameters[20];

Shader* Shader::nextFreeShader()
{
	return &assignedShaders[numShadersAssigned++];
}

Shader::Shader()
{
	this->numUniformParameters = 0;
}

void Shader::sendGlobalUniforms( const Shader * shad )
{
	for( unsigned int i = 0; i < numGlobalUniformParameters; i++ )
	{
		GLint loc = glGetUniformLocation(shad->shaderProgramID, globalUniformParameters[ i ].name );
		globalUniformParameters[ i ].location = loc;
		globalUniformParameters[ i ].send();
	}
}

void Shader::sendUniforms() const
{
	sendGlobalUniforms( this );

	for( unsigned int i = 0; i < numUniformParameters; i++ )
	{
		uniformParameters[i].send();
	}
}

void Shader::addUniformParameter( const char* name, ParameterType parameterType, const void* value)
{
	GLint loc = glGetUniformLocation(shaderProgramID, name);
	uniformParameters[ numUniformParameters ].location = loc;
	uniformParameters[ numUniformParameters ].data = value;
	uniformParameters[ numUniformParameters ].type = parameterType;
	numUniformParameters++;
}

void Shader::setUniformParameter( const char* name, ParameterType parameterType, const void* value)
{
	glUseProgram( shaderProgramID );
	GLint loc = glGetUniformLocation(shaderProgramID, name);
	UniformInfo inf;
	inf.location = loc;
	inf.data = value;
	inf.type = parameterType;
	inf.send();
}

void Shader::addGlobalUniformParameter( const char* name, ParameterType parameterType, const void* value)
{
	globalUniformParameters[ numGlobalUniformParameters++ ] = UniformInfo( name, value, parameterType );
}

void Shader::setGlobalUniformParameter( const char* name, ParameterType parameterType, const void* value)
{
	for( unsigned int i = 0; i < numShadersAssigned; i++ )
	{
		assignedShaders[i].setUniformParameter( name, parameterType, value );
	}
}