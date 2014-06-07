#include <GL/glew.h>
#include "OpenGLWindow.h"
#include <glm\glm.hpp>
#include <ShapeGenerator.h>
#include <iostream>


Neumont::ShapeData torus, sphere;
GLuint program;

const GLchar * VERTEXSHADER[] =
{
	"in vec4 (layout=0) vPosition",
	"in vec4 (layout=1) vColor",
	"out vec4 oPosition",
	"uniform mat4 world",
	"uniform mat4 pv",
	"main()",
	"{",
	" oPosition = vPosition;",
	" gl_Position = pv * world * vPosition;",
	"}",
	" ",
};

const GLchar * FRAGMENTSHADER[] =
{
	"in vec4 oPosition",
	"out vec4 glColor",
	"main()",
	"{",
	" glColor = oPosition;",
	"}",
	" ",
	" ",
	" ",
};

void setUpShaders()
{
	GLuint vshad = glCreateShader( GL_VERTEX_SHADER );
	GLuint fshad = glCreateShader( GL_FRAGMENT_SHADER );

	glShaderSource( vshad, 1, &VERTEXSHADER[0], NULL );
	glShaderSource( fshad, 1, &FRAGMENTSHADER[0], NULL );

	glCompileShader( vshad );
	glCompileShader( fshad );

	GLuint program = glCreateProgram();
	glAttachShader( program, vshad );
	glAttachShader( program, fshad );

	glLinkProgram( program );

	std::cout << "end" << std::endl;
}

void setUpHardware()
{
	GLuint vbuf, ibuf;
	glGenBuffers( 1, &vbuf );
	glGenBuffers( 1, &ibuf );

	glBindBuffer( GL_ARRAY_BUFFER, vbuf );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ibuf );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, (void*)Neumont::Vertex::POSITION_OFFSET );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, (void*)Neumont::Vertex::POSITION_OFFSET );
}

void loadModelData()
{
	torus = Neumont::ShapeGenerator::makeTorus( 3 );

	glBufferData( GL_ARRAY_BUFFER, torus.vertexBufferSize(), torus.verts, GL_STATIC_DRAW );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, torus.indexBufferSize(), torus.indices, GL_STATIC_DRAW );
}

void OpenGLWindow::initializeGL()
{
	glewInit();
	setUpShaders();
	setUpHardware();
	loadModelData();
}

void OpenGLWindow::paintGL()
{
	GLint loc = glGetUniformLocation(program, "world");
	if (loc != -1)
	{
		//glUniformMatrix4fv( loc, 1, false, glm::mat4()) );
	}
	glDrawElements( GL_TRIANGLES, torus.numIndices, GL_SHORT, torus.indices );
}
	