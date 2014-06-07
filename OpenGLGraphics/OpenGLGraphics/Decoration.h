#pragma once
#include "Model.h"
#include "glm\glm.hpp"
#include "RandomHelper.h"
#include <iostream>

class Decoration
{
	static Model * model;
	glm::vec3 position;
	int index;

public:
	inline Decoration()
	{
		index = RandomHelper::random( 3 );

		std::cout << index << std::endl;
	}

	static void initialize();

	void draw( GLuint shader );

	inline glm::vec3 getPosition()
	{
		return position;
	}

	inline void setPosition(glm::vec3 position)
	{
		this->position = position;
	}
};

