#pragma once
#include "definition.h"
#include "UniformInfo.h"

struct EXPORT Shader
{
	unsigned int shaderProgramID;

	Shader();

	static void sendGlobalUniforms( const Shader * shad );
	void sendUniforms() const;

	void addUniformParameter( const char* name, ParameterType parameterType, const void* value);
	void setUniformParameter( const char* name, ParameterType parameterType, const void* value);

	static void addGlobalUniformParameter( const char* name, ParameterType parameterType, const void* value);
	static void setGlobalUniformParameter( const char* name, ParameterType parameterType, const void* value);

private:
	friend class ContentManager;
	static Shader* nextFreeShader();

	unsigned int numUniformParameters;
	UniformInfo uniformParameters[20];

	
};