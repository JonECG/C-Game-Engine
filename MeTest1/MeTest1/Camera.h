#pragma once

#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <GL\glew.h>

class Camera
{
	glm::vec3 from, to;
	GLfloat aspect, camNear, camFar;
public:
	inline Camera()
	{
		aspect = 1.0f;
		camNear = 0.1f;
		camFar = 200.0f;
	}

	inline Camera( GLfloat aspect, GLfloat camNear, GLfloat camFar)
	{
		this -> aspect = aspect;
		this -> camNear = camNear;
		this -> camFar = camFar;
	}

	inline void updateProjection( GLuint shaderProgram, char* uniformName )
	{
		GLint loc = glGetUniformLocation(shaderProgram, uniformName);
		if (loc != -1)
		{
			glm::mat4 proj = calcProjection();
			glUniformMatrix4fv( loc, 1, false, (float*)&proj );
		}
	}

	inline glm::mat4 calcProjection()
	{
		return glm::perspective(50.0f, aspect, camNear, camFar);
	}

	inline void updateModelView( GLuint shaderProgram, char* uniformName )
	{
		GLint loc = glGetUniformLocation(shaderProgram, uniformName);
		if (loc != -1)
		{
			glm::mat4 mv = calcModelView();
			glUniformMatrix4fv( loc, 1, false, (float*)&mv );
		}
	}

	inline glm::mat4 calcModelView()
	{
		return glm::lookAt( from, to, glm::vec3( 0.0, 1.0, 0.0 ) );
	}

	inline void updateModelViewProjection( GLuint shaderProgram, char* uniformName )
	{
		GLint loc = glGetUniformLocation(shaderProgram, uniformName);
		if (loc != -1)
		{
			glm::mat4 mvp = calcModelViewProjection();
			glUniformMatrix4fv( loc, 1, false, (float*)&mvp );
		}
	}

	inline glm::mat4 calcModelViewProjection()
	{
		return calcProjection()*calcModelView();
	}

	inline glm::mat4 calcInverseModelViewProjection()
	{
		return glm::inverse( calcModelViewProjection() );
	}

	inline glm::vec3 getFrom(){ return from; }
	inline glm::vec3 getTo(){ return to; }
	inline void setFrom( glm::vec3 from ){ this -> from = from; }
	inline void setTo( glm::vec3 to ){ this -> to = to; }
};