#pragma once

#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include <GL\glew.h>
#include "Engine.h"

class Camera
{
	
	GLfloat aspect, camNear, camFar;
public:
	glm::vec3 from, to;
	glm::vec3 up;
	glm::mat4 proj, view, mvp;
	inline Camera()
	{
		up = glm::vec3( 0, 1, 0 );
		aspect = 1.0f;
		camNear = 0.1f;
		camFar = 200.0f;
	}

	inline Camera( GLfloat aspect, GLfloat camNear, GLfloat camFar)
	{
		up = glm::vec3( 0, 1, 0 );
		this -> aspect = aspect;
		this -> camNear = camNear;
		this -> camFar = camFar;
	}

	inline void move( glm::vec3 move )
	{
		glm::vec3 localZ = glm::normalize( to - from );
		glm::vec3 localX = glm::cross( localZ, up );
		glm::vec3 localY = glm::cross( localX, localZ );

		glm::mat3 localTransform = glm::mat3( localX, localY, localZ );
		glm::vec3 transformMove = localTransform * move;

		from += transformMove;
		to += transformMove;
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
		proj = glm::perspective(50.0f, aspect, camNear, camFar);
		return proj;
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
		view = glm::lookAt( from, to, up );
		return view;
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
		mvp = calcProjection()*calcModelView();
		return mvp;
	}

	inline glm::mat4 calcInverseModelViewProjection()
	{
		return glm::inverse( calcModelViewProjection() );
	}

	inline glm::vec3 getFrom(){ return from; }
	inline glm::vec3 getTo(){ return to; }
	inline void setAspect( GLfloat aspect ){ this -> aspect = aspect; }
	inline void setFrom( glm::vec3 from ){ this -> from = from; }
	inline void setTo( glm::vec3 to ){ this -> to = to; }
};