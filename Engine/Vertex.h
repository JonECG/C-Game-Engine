#pragma once
#include <glm\glm.hpp>
struct Vertex
{
	glm::vec3 position, color, normal;
	glm::vec2 uv;
	//glm::vec3 tangent, binormal;

	inline Vertex()
	{
	}
	inline Vertex( glm::vec3 position, glm::vec3 color, glm::vec3 normal, glm::vec2 uv )
	{
		this -> position = position;
		this -> color = color;
		this -> normal = normal;
		this -> uv = uv;
	}
	static void setUpAttributes();

	inline static Vertex avg( Vertex &v1, Vertex &v2, GLfloat ratio )
	{
		GLfloat tot = ratio + 1;
		return Vertex( 
			(v1.position*ratio + v2.position)/tot,
			(v1.color*ratio + v2.color)/tot,
			glm::normalize( (v1.normal*ratio + v2.normal)/tot ),
			(v1.uv*ratio + v2.uv)/tot );
	}

};