#pragma once
#include "definition.h"

enum ParameterType;
struct Renderable;
struct Shader;
struct Texture;

struct EXPORT Geometry
{
	unsigned int vertexArrayID;
	unsigned int numIndices;
	unsigned int indexingMode;
	unsigned int bufferOffset;

	void addAttribute( unsigned int layoutLocation, ParameterType parameterType, unsigned int bufferOffset, unsigned int bufferStride);
	Renderable* makeRenderable( Shader* shader, const Texture* texture = nullptr, bool visible = true );
	//inline Geometry( unsigned int vertexArrayID, unsigned int numIndices, unsigned int indexingMode, unsigned int bufferOffset );
};