#pragma once
#include <GL\glew.h>
#include "Vertex.h"
#include "ShapeData.h"
class Model
{
	static GLuint endOfBuffer;
	GLuint modelOffset;
	static bool initialized;
	bool assigned;

	Vertex * vertices;
	GLushort * indices;
	GLuint vSize, iSize;
	void assignModelOffset();
	void populateIndices( GLuint size );

public:
	
	static void initialize();

	inline Model(){ vertices = NULL; indices = NULL; vSize = 0; iSize = 0; }
	Model(glm::vec3 * vertices, GLuint size);
	Model(Vertex * vertices, GLuint size);
	Model(Neumont::ShapeData shape);
	Model(Vertex * vertices, GLuint vSize, GLushort * indices, GLuint iSize );
	void draw();
	inline GLuint getSize(){ return iSize; }
	inline Vertex * getVertices(){ return vertices; }
	//static Model loadFromFile();
};

