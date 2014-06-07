#include "Model.h"
#include <GL\glew.h>
#include <iostream>


GLuint Model::endOfBuffer = 0;
bool Model::initialized = false;

void Model::initialize()
{
	glBufferData(GL_ARRAY_BUFFER, (1000000) * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
}

void Model::assignModelOffset()
{
	if ( !initialized )
	{
		initialize();
		initialized = true;
	}
	modelOffset = endOfBuffer;

	for( int i = 0; i < iSize; i++ )
	{
		indices[i] += modelOffset/sizeof(Vertex);
	}

	glBufferSubData( GL_ARRAY_BUFFER, modelOffset, vSize*sizeof(Vertex), vertices );
	glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, modelOffset+vSize*sizeof(Vertex), iSize*sizeof(GLshort), indices );

	endOfBuffer += vSize * sizeof( Vertex ) + ( ( iSize * sizeof( GLshort ) ) / sizeof( Vertex ) + 1 ) * sizeof( Vertex );
	assigned = true;
	std::cout << "Assigned " << iSize << " indices model to Buffer Pos " << modelOffset << " with the size of " << vSize * sizeof( Vertex ) + iSize * sizeof( GLshort ) << " bytes." << std::endl;
}

void Model::populateIndices( GLuint size )
{
	indices = new GLushort [size];
	for( int i = 0; i < size; i ++ )
	{
		indices[i] = i;
	}
	iSize = size;
	std::cout << "Populated " << size << std::endl;
}

Model::Model(glm::vec3 * vectors, GLuint size)
{
	this -> vertices = (Vertex*)vectors;
	this -> vSize = size;
	assigned = false;
	populateIndices(size);
}

Model::Model(Vertex * vertices, GLuint size)
{
	this -> vertices = vertices;
	this -> vSize = size;
	assigned = false;
	populateIndices(size);
}

Model::Model(Neumont::ShapeData shape)
{
	this -> vertices = new Vertex[ shape.numVerts ];
	for( int i = 0; i < shape.numVerts; i++ )
	{
		this->vertices[i].position = shape.verts[i].position;
		this->vertices[i].normal = shape.verts[i].normal;
		this->vertices[i].color = glm::vec3( shape.verts[i].color );
		this->vertices[i].uv = shape.verts[i].uv;
		this->vertices[i].binormal = shape.verts[i].normal;
		this->vertices[i].tangent = shape.verts[i].normal;
	}
	this -> vSize = shape.numVerts;
	this -> indices = shape.indices;
	this -> iSize = shape.numIndices;
	assigned = false;
}

Model::Model(Vertex * vertices, GLuint vSize, GLushort * indices, GLuint iSize )
{
	this -> vertices = vertices;
	this -> vSize = vSize;
	this -> indices = indices;
	this -> iSize = iSize;
	assigned = false;
}

void Model::draw()
{
	if ( vertices != NULL && indices != NULL && iSize != 0 && vSize != 0 )
	{
		if( !assigned )
		{
			assignModelOffset();
		}

		//std::cout<< "Drawing " << iSize << std::endl;
		glDrawElements( GL_TRIANGLES, iSize, GL_UNSIGNED_SHORT, (void*)(modelOffset+vSize*sizeof(Vertex)) );
	}
}
