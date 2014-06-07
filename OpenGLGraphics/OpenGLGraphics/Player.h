#pragma once
#include "Model.h"
#include "Sierpinski.h"
#include "glm\glm.hpp"
#include "Hill.h"

class Player
{
	static Model model;
	GLfloat scale;
	glm::vec3 position, velocity, acceleration;
	GLfloat direction;

public:
	Player();
	static void initialize();
	void update( GLfloat dt, GLshort turnLeft, GLshort turnRight, GLshort forward, GLshort backward );
	void draw( GLuint shader );

	glm::vec3 getPosition();
	GLfloat getDirection();
	inline GLfloat getScale(){ return scale; }

	void Player::collide( GLfloat dt, Hill hill );
	inline void setPosition(glm::vec3 position){ this->position = position; }
};

