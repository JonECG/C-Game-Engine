#pragma once
#include "definition.h"
#include <glm\glm.hpp>

struct Geometry;

struct Vertex
{
	float data[12];

	static EXPORT unsigned int POSITION_OFFSET;
	static EXPORT unsigned int COLOR_OFFSET;
	static EXPORT unsigned int NORMAL_OFFSET;
	static EXPORT unsigned int UV_OFFSET;
	static EXPORT unsigned int STRIDE;

	static EXPORT void setAttributes( Geometry* geo );

	glm::vec3 getPosition();
	glm::vec4 getColor();
	glm::vec3 getNormal();
	glm::vec2 getUv();

	void setPosition( glm::vec3 position );
	void setColor( glm::vec4 color );
	void setNormal( glm::vec3 normal );
	void setUv( glm::vec2 uv );
};

