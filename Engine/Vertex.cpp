#include <GL\glew.h>
#include "Vertex.h"

void Vertex::setUpAttributes()
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
}