#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <GL\glew.h>
#include "Model.h"
#include "RandomHelper.h"

class Terrain
{

	GLfloat* heights;
	GLint xSegments, ySegments;
	glm::vec3 position, dimensions;
	Model model;

	void smoothHeights( int radius, int iterations );
	void normalizeHeights();
	void generateModel();


public:
	Terrain();
	~Terrain();

	static Terrain createRandom(glm::vec3 position, glm::vec3 dimensions, GLint xSegments, GLint ySegments );

	static Terrain createFromHeightmap(char * fileName, glm::vec3 position, glm::vec3 dimensions, GLint xSegments, GLint ySegments );

	

	glm::vec3 fitPosition( glm::vec3 position );
	inline void draw( GLuint shaderProgram )
	{
		GLuint loc = glGetUniformLocation(shaderProgram, "world");
		if (loc != -1)
		{
			glUniformMatrix4fv( loc, 1, false, (float*)&glm::translate(glm::mat4(),position) );
		}
		model.draw();
	}

	inline glm::vec3 calcRandomPosition()
	{
		return fitPosition( glm::vec3( 
			RandomHelper::random( dimensions.x ), 
			RandomHelper::random( dimensions.y ), 
			RandomHelper::random( dimensions.z ) ) + position - dimensions/2.0f );
	}
};

