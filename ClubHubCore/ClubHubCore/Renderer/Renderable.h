#pragma once
#include "definition.h"
#include "UniformInfo.h"
struct Shader;
struct Geometry;
struct Texture;
enum ParameterType;

struct Renderable
{
	Geometry* whatGeometry;
	Shader* howShader;
	bool visible;
	const Texture *texture, *optionalTexture;

	Renderable();

	void EXPORT sendUniforms() const;

	void addUniformParameter( const char* name, ParameterType parameterType, const void* value);
	void setUniformParameter( const char* name, ParameterType parameterType, const void* value);

	static Renderable * nextFreeRenderable();

	EXPORT Renderable* makeCopy() const;

private:
	unsigned int numUniformParameters;
	UniformInfo uniformParameters[10];
};

