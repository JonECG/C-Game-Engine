#include <GL/glew.h>
#include "OpenGLWindow.h"
#include <glm\glm.hpp>
#include <ShapeGenerator.h>
#include <iostream>
#include "Camera.h"
#include <glm\gtc\matrix_transform.hpp>
#include <Qt\qtimer.h>

Neumont::ShapeData torus, sphere, teapot, cube, plane;
int torusOff, sphereOff, teapotOff, cubeOff, planeOff;

GLuint program;
Camera camera;

QTimer timer;

using glm::vec3;

const GLchar * VERTEXSHADER =
	"#version 400\r\n"
	"layout (location=0) in vec4 vPosition;"
	"layout (location=1) in vec4 vColor;"
	"out vec4 posColor;"
	"uniform int mode;"
	"uniform mat4 world;"
	"uniform mat4 proj;"
	"uniform mat4 view;"
	"void main()"
	"{"
	" posColor = vColor;"
	" if( mode == 2 ) { posColor = mat4(mat3(world))*posColor; }"
	" if( mode == 3 ) { posColor = world*posColor; }"
	" if( mode == 4 ) { posColor = view * world * posColor; posColor.z *= -1; }"
	" if( mode == 5 ) { posColor = proj * view * world * posColor; }"
	" gl_Position = proj * view * world * vPosition;"
	"}";

const GLchar * FRAGMENTSHADER =

	"#version 400\r\n"
	"in vec4 posColor;"
	"out vec4 glColor;"
	"uniform int mode;"
	"void main()"
	"{"
	" if( mode == 5 ) { glColor = posColor/posColor.w; }"
	" if( mode != 5 ) { glColor = vec4(vec3(posColor), 1.0); }"
	"}"
	" "
	" "
	" ";

void setUpShaders()
{
	GLuint vshad = glCreateShader( GL_VERTEX_SHADER );
	GLuint fshad = glCreateShader( GL_FRAGMENT_SHADER );

	glShaderSource( vshad, 1, &VERTEXSHADER, NULL );
	glShaderSource( fshad, 1, &FRAGMENTSHADER, NULL );

	glCompileShader( vshad );
	glCompileShader( fshad );


	program = glCreateProgram();
	glAttachShader( program, vshad );
	glAttachShader( program, fshad );

	glLinkProgram( program );
	glUseProgram( program );

	std::cout << "end" << std::endl;
}

void setUpHardware()
{
	GLuint vbuf, ibuf;
	glGenBuffers( 1, &vbuf );
	glGenBuffers( 1, &ibuf );

	glBindBuffer( GL_ARRAY_BUFFER, vbuf );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vbuf );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, (void*)Neumont::Vertex::POSITION_OFFSET );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, Neumont::Vertex::STRIDE, (void*)Neumont::Vertex::POSITION_OFFSET );
}

int loadModel( Neumont::ShapeData shape )
{
	static int offset = 0;

	glBufferSubData( GL_ARRAY_BUFFER, offset, shape.vertexBufferSize(), shape.verts );

	for( int i = 0; i < shape.numIndices; i++ )
	{
		shape.indices[i] += offset / sizeof( Neumont::Vertex );
	}

	glBufferSubData( GL_ARRAY_BUFFER, offset + shape.vertexBufferSize(), shape.indexBufferSize(), shape.indices );

	int result = offset;

	offset += shape.vertexBufferSize() + (shape.indexBufferSize() / sizeof( Neumont::Vertex ) + 1) * sizeof( Neumont::Vertex );

	return result;
}

void loadModelData()
{
	glBufferData( GL_ARRAY_BUFFER, 1000000, 0, GL_STATIC_DRAW );

	torus = Neumont::ShapeGenerator::makeTorus( 10 );
	sphere = Neumont::ShapeGenerator::makeSphere( 10 );
	teapot = Neumont::ShapeGenerator::makeTeapot( 10, glm::mat4() );
	plane = Neumont::ShapeGenerator::makePlane(5);
	cube = Neumont::ShapeGenerator::makeCube();

	torusOff = loadModel( torus );
	sphereOff = loadModel( sphere );
	teapotOff = loadModel( teapot );
	planeOff = loadModel( plane );
	cubeOff = loadModel( cube );
}

void OpenGLWindow::initializeGL()
{
	glewInit();
	setUpShaders();
	setUpHardware();
	loadModelData();
	paintGL();

	camera.setFrom( glm::vec3( 0, 0, 15 ) );
	camera.setTo( glm::vec3( 0,0,0 ) );

	glEnable( GL_DEPTH_TEST );

	connect( &timer, SIGNAL( timeout() ), this, SLOT( updateWindow() ) );

	timer.start(0);
}

void OpenGLWindow::updateWindow()
{
	int horMove = ( ( GetAsyncKeyState('D') != 0 ) ? 1 : 0 ) - ( ( GetAsyncKeyState('A') != 0 ) ? 1 : 0 );
	int vertMove = ( ( GetAsyncKeyState('S') != 0 ) ? 1 : 0 ) - ( ( GetAsyncKeyState('W') != 0 ) ? 1 : 0 );
	vec3 change = vec3( horMove, 0, vertMove ) * 0.1f;
	camera.setFrom( camera.getFrom() + change );
	camera.setTo( camera.getTo() + change );

	repaint();
}

void OpenGLWindow::paintGL()
{
	glViewport( 0,0, width(), height() );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	static float length = 0;
	length++;
	camera.setAspect( ((float)width())/height() );
	camera.updateProjection( program, "proj" );
	camera.updateModelView( program, "view" );

	GLint loc = glGetUniformLocation(program, "mode");

	//Color based on Model position
	if ( GetAsyncKeyState('1') )
		glUniform1i( loc, 1 );

	//Color based on World (sans translation) position
	if ( GetAsyncKeyState('2') )
		glUniform1i( loc, 2 );

	//Color based on World (with translation) position
	if ( GetAsyncKeyState('3') )
		glUniform1i( loc, 3 );

	//Color based on View position
	if ( GetAsyncKeyState('4') )
		glUniform1i( loc, 4 );

	//Color based on Projection position
	if ( GetAsyncKeyState('5') )
		glUniform1i( loc, 5 );



	loc = glGetUniformLocation(program, "world");

	glUniformMatrix4fv( loc, 1, false, (float*)&(glm::translate(vec3( 1, 0, 0) )* glm::rotate(glm::mat4(),length,glm::normalize(vec3(1,1,0)))));
	glDrawElements( GL_TRIANGLES, torus.numIndices, GL_UNSIGNED_SHORT, (void*)(torusOff+torus.vertexBufferSize()) );

	glUniformMatrix4fv( loc, 1, false, (float*)&(glm::translate(vec3( 3, 2, 0) )*glm::rotate(glm::mat4(),length,glm::normalize(vec3(0,1,1)))));
	glDrawElements( GL_TRIANGLES, sphere.numIndices, GL_UNSIGNED_SHORT, (void*)(sphereOff+sphere.vertexBufferSize()) );

	glUniformMatrix4fv( loc, 1, false, (float*)&(glm::translate(vec3( -5, 1, 1) )*glm::rotate(glm::mat4(),length,glm::normalize(vec3(1,0.5f,0)))));
	glDrawElements( GL_TRIANGLES, teapot.numIndices, GL_UNSIGNED_SHORT, (void*)(teapotOff+teapot.vertexBufferSize()) );

	glUniformMatrix4fv( loc, 1, false, (float*)&(glm::translate(vec3( 1, 4, 0) )*glm::rotate(glm::mat4(),length,glm::normalize(vec3(1,0.5f,0)))));
	glDrawElements( GL_TRIANGLES, cube.numIndices, GL_UNSIGNED_SHORT, (void*)(cubeOff+cube.vertexBufferSize()) );

	glUniformMatrix4fv( loc, 1, false, (float*)&(glm::translate(vec3( -1, -3, 0) )*glm::rotate(glm::mat4(),length,glm::normalize(vec3(1,0.5f,0)))));
	glDrawElements( GL_TRIANGLES, plane.numIndices, GL_UNSIGNED_SHORT, (void*)(planeOff+plane.vertexBufferSize()) );
}
	