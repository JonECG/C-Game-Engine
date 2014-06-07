#pragma once
#include <glm\glm.hpp>
#include "Model.h"
class Sierpinski
{
public:
	static glm::vec3* generateSierpinskiTriangle( int iterations );
	static int sierpinskiTriangleStep( int iterations, glm::vec3* triangles, glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, int index  );

	static Model generateSierpinskiPyramid( int iterations, glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, glm::vec3& p4 );
	static int sierpinskiPyramidStep( int iterations, glm::vec3* triangles, glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, glm::vec3& p4, int index  );
};

