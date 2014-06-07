#pragma once
#include "Model.h"
#include "glm\glm.hpp"
#include "Terrain.h"

class Hill
{
	float time;
	static Model ring;
	float wave;
	glm::vec3 position;

public:
	Hill();
	static void initialize();
	void update( GLfloat dt, Terrain &terrain );
	void draw( GLuint shader );

	glm::vec3 getPosition();
	inline float getScale(){ return time/4.0f; }

	inline void setPosition(glm::vec3 position){ this->position = position; }
};

