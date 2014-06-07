#include "Player.h"
#include "OpenGlWindow.h"
#include <glm\gtc\matrix_transform.hpp>

using glm::vec2;
using glm::vec3;

Model Player::model = Model(); 

Player::Player(void)
{
	scale = 1;
	direction = 0;
}

void Player::initialize()
{
	float radius = 0.73f;
	model = Sierpinski::generateSierpinskiPyramid( 5, vec3( cos(3.14*240/180)*radius, 0, sin(3.14*240/180)*radius ), vec3( cos(3.14*120/180)*radius, 0, sin(3.14*120/180)*radius ), vec3( radius, 0 , +0.0 ), vec3( +0.0f, radius, +0.0f ) );
}

void Player::update( GLfloat dt, GLshort turnLeft, GLshort turnRight, GLshort forward, GLshort backward )
{
	direction += ((turnRight?1:0) - (turnLeft?1:0))*2*dt;
	
	acceleration = vec3( cos(direction), 0, sin(direction) ) * ((forward?1.0f:0.0f) - (backward?1.0f:0) );
	acceleration *= 0.3f * dt * getScale();

	velocity += acceleration;
	position += velocity;

	velocity *= .95f;
}

void Player::collide( GLfloat dt, Hill hill )
{
	if ( glm::length( position - hill.getPosition() ) < ( scale*0.9f + hill.getScale()*1.0f ) )
	{
		//scale += 0.1f*dt;
		//std::cout << scale << std::endl;
	}
}

void Player::draw( GLuint shaderProgram )
{
	GLuint loc = glGetUniformLocation(shaderProgram, "world");
	if (loc != -1)
	{
		glm::mat4 subRotate;
		subRotate[0][0] = cos(direction);
		subRotate[0][2] = sin(direction);
		subRotate[2][0] = -sin(direction);
		subRotate[2][2] = cos(direction);

		glm::mat4 transform = glm::translate( glm::mat4(), vec3(position.x, position.y, position.z) ) * subRotate * glm::scale( vec3( getScale() ) );

		glUniformMatrix4fv( loc, 1, false, (float*)&transform );
	}

	model.draw();
}

vec3 Player::getPosition()
{
	return position;
}

GLfloat Player::getDirection()
{
	return direction;
}