#pragma once
#include <glm\glm.hpp>
struct Vertex
{
	glm::vec3 position, color, normal;
	glm::vec2 uv;
	glm::vec3 binormal, tangent;

	inline Vertex()
	{
	}
	
	inline Vertex( glm::vec3 position, glm::vec3 color, glm::vec3 normal, glm::vec2 uv )
	{
		this -> position = position;
		this -> color = color;
		this -> normal = normal;
		this -> uv = uv;
		this -> binormal = normal;
		this -> tangent = normal;
	}

	inline Vertex( glm::vec3 position, glm::vec3 color, glm::vec3 normal, glm::vec2 uv, glm::vec3 binormal, glm::vec3 tangent )
	{
		this -> position = position;
		this -> color = color;
		this -> normal = normal;
		this -> uv = uv;
		this -> binormal = binormal;
		this -> tangent = tangent;
	}
	inline static void setUpAttributes()
	{
		//Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0 );

		//Color
		glEnableVertexAttribArray(1);
		glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)) );

		//Normal
		glEnableVertexAttribArray(2);
		glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2*sizeof(glm::vec3)) );

		//UV
		glEnableVertexAttribArray(3);
		glVertexAttribPointer( 3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(glm::vec3)) );

		//Binormal
		glEnableVertexAttribArray(4);
		glVertexAttribPointer( 4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(glm::vec3)+sizeof(glm::vec2)) );

		//Tangent
		glEnableVertexAttribArray(5);
		glVertexAttribPointer( 5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(4*sizeof(glm::vec3)+sizeof(glm::vec2)) );
	}

	inline static Vertex avg( Vertex &v1, Vertex &v2, GLfloat ratio )
	{
		GLfloat tot = ratio + 1;
		return Vertex( 
			(v1.position*ratio + v2.position)/tot,
			(v1.color*ratio + v2.color)/tot,
			glm::normalize( (v1.normal*ratio + v2.normal)/tot ),
			(v1.uv*ratio + v2.uv)/tot,
			(v1.binormal*ratio + v2.binormal)/tot,
			(v1.tangent*ratio + v2.tangent)/tot);
	}


};