#include <GL\glew.h>
#include "OpenGlWindow.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <iostream>
#include "Sierpinski.h"
#include "GLHelper.h"
#include "Model.h"
#include "Timer.h"
#include "Terrain.h"
#include "Hill.h"
#include "Camera.h"
#include <QtGui\QMouseEvent>
#include <Qt\qimagereader.h>
#include <glm\gtc\matrix_inverse.hpp>
#include "Decoration.h"
#include <ShapeGenerator.h>

#define OFFSET(a) (void*)(a)

using glm::vec4;
using glm::vec3;
using glm::vec2;

int numberOfVertices;

GLuint shaderProgram;
GLuint passProgram;

GLuint texture;
//Model pyramid;
//Model ground;

Timer timing;

Player playerOne, playerTwo;
Camera cameraOne, cameraTwo;

Hill hill;

vec2 position, vel;
vec3 worldMouse;
vec4 mouse;

Model cube;

Terrain ter;

Decoration * decorations;


void OpenGlWindow::setUpShaders()
{
	shaderProgram = GLHelper::loadShader( "res/normal.vert", "res/normal.frag" );
	passProgram = GLHelper::loadShader( "res/pass.vert", "res/pass.frag" );
	int i = 0;
	glUseProgram( shaderProgram );
}

void OpenGlWindow::initializeGL()
{
	glewInit();

	setUpShaders();

	GLuint vertexBufferID;
	glGenBuffers( 1, &vertexBufferID );
	glBindBuffer( GL_ARRAY_BUFFER, vertexBufferID );

	//GLuint elementBufferID;
	//glGenBuffers( 1, &elementBufferID );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, vertexBufferID );

	Model::initialize();

	cube = Model( Neumont::ShapeGenerator::makeCube() );
	ter = Terrain();
	mouse = vec4();
	worldMouse = vec3();
	
	Decoration::initialize();

	Vertex::setUpAttributes();

	Hill::initialize();
	//Player::initialize();

	QImage image;
	image.load( "res/myColorTexture.png" );
	//QImageReader imageReader( "res/text.gif" );
	//imageReader.read( &image );
	//std::cout << imageReader.error();
	//std::cout << imageReader.errorString().data();

	//QImage image = QImage( 16, 16, QImage::Format_RGB32 );
	//image.fill( Qt::green );

	image = convertToGLFormat( image );

	glEnable( GL_TEXTURE_2D );
	glActiveTexture( GL_TEXTURE0 );

	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
	
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	GLint loc = glGetUniformLocation( shaderProgram, "colorTexture");
	glUniform1i(loc, 0);

	ter = Terrain::createFromHeightmap( "res/heightmap.png", vec3(0,0,0), vec3( 50.0, 30.0f, 50.0 ), 30, 30  );
	//ter = Terrain::createRandom( vec3(), vec3( 50.0, 20.0f, 50.0 ), 60, 60 );

	decorations = new Decoration[100];
	for( int i = 0; i < 100; i++ )
	{
		decorations[i].setPosition( ter.calcRandomPosition() );
	}

	

	glEnable( GL_DEPTH_TEST );
	glDisable( GL_DEPTH_RANGE );
	glDepthMask(true);
	glDisable( GL_CULL_FACE );

	cameraOne.setFrom( vec3(30, 20, 30) );
	cameraOne.setTo( vec3(0,0,0) );

	connect( &timer, SIGNAL(timeout()), this, SLOT(update()) );
	timer.start(0);
}

void OpenGlWindow::mousePressEvent( QMouseEvent * mEvent )
{

	mouse.x = //mEvent->x();
		(mEvent->x()%(width()/2))/(width()/4.0f)-1;
	mouse.y = //mEvent->y();;
		-(mEvent->y()/(height()/4.0f)-1);
	glReadPixels(mEvent->x(), (height()/2)-mEvent->y(), 1, 1,GL_DEPTH_COMPONENT,GL_FLOAT, &(mouse.z));
	//glReadPixels(mEvent->x(), 640-mEvent->y(), 1, 1,GL_RED,GL_FLOAT, &(mouse.w));

	//worldMouse = glm::unProject( vec3(mouse), cameraOne.calcModelView(), cameraOne.calcProjection(), vec4( 0, 0, 640, 640 ) );

	//std::cout << mouse.x << " " << mouse.y << " " << mouse.z << " " << mouse.w << std::endl;
	mouse.w *= 40;
	mouse.w = 1;
	mouse.z = (mouse.z*2-1);
	//std::cout << mouse.x << " " << mouse.y << " " << mouse.z << " " << mouse.w << std::endl;
	//mouse.x *= mouse.w;
	//mouse.y *= mouse.w;
	//mouse.z *= mouse.w;
	

	glm::mat4 invMat;
	if (mEvent->x() < (width()/2))
		invMat = cameraOne.calcInverseModelViewProjection();
	else
		invMat = cameraTwo.calcInverseModelViewProjection();

	vec4 temp = invMat * mouse;
	worldMouse = vec3( temp )/temp.w;

	//std::cout << "next " << mouse.x << " " << mouse.y << " " << mouse.z << " " << temp.w << std::endl;

	//playerTwo.setPosition( worldMouse );
	
}

void OpenGlWindow::update()
{
	GLfloat dt = timing.interval();
	static GLfloat time = 0;
	time += dt;

	GLuint loc = glGetUniformLocation(shaderProgram, "time");
	if (loc != -1)
		glUniform1f( loc, time);

	cameraOne.move( vec3(
		(GetAsyncKeyState('D')?1:0) - (GetAsyncKeyState('A')?1:0),
		(GetAsyncKeyState('W')?1:0) - (GetAsyncKeyState('S')?1:0),
		(GetAsyncKeyState('E')?1:0) - (GetAsyncKeyState('Q')?1:0)
		) * 8.0f * dt );

	/*playerOne.update( dt, GetAsyncKeyState( 'A' ), GetAsyncKeyState( 'D' ), GetAsyncKeyState( 'W' ), GetAsyncKeyState( 'S' ) );
	playerOne.setPosition( ter.fitPosition( playerOne.getPosition() ) );
	playerOne.collide( dt, hill );

	playerTwo.update( dt, GetAsyncKeyState( VK_LEFT ), GetAsyncKeyState( VK_RIGHT ), GetAsyncKeyState( VK_UP ), GetAsyncKeyState( VK_DOWN ) );
	playerTwo.setPosition( ter.fitPosition( playerTwo.getPosition() ) );
	playerTwo.collide( dt, hill );*/

	/*if ( GetAsyncKeyState( 'V' ) )
	{
		vec3 in = playerTwo.getPosition();
		std::cout << "vecin: " << in.x << ", " << in.y << ", " << in.z << std::endl;
		vec4 temp = cameraOne.calcModelViewProjection() * vec4( in, 1 );
		std::cout << "vecout: " << temp.x << ", " << temp.y << ", " << temp.z << ", " << temp.w << std::endl;
		std::cout << "vecoutdw: " << temp.x/temp.w << ", " << temp.y/temp.w << ", " << temp.z/temp.w << std::endl;
		in = vec3( cameraOne.calcInverseModelViewProjection() * temp );
		std::cout << "vecrein: " << in.x << ", " << in.y << ", " << in.z << std::endl << std::endl;
	}*/
	
	hill.update( dt, ter );

	repaint();
}

void OpenGlWindow::paintGL()
{
	glClearColor( 0.0f, 0.4f, 0.6f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glViewport( 0, 0, width(), height() );

	glBindTexture(GL_TEXTURE_2D, texture);

	cameraOne.setAspect( width()/ height() );

	/*glViewport( 0, 0, width()/2, height() );
	paintPort( playerOne, cameraOne );

	glViewport( width()/2, 0, width()/2, height() );
	paintPort( playerTwo, cameraTwo );*/

	cameraOne.updateModelViewProjection( shaderProgram, "modelViewProjection" );


	GLuint loc = glGetUniformLocation(shaderProgram, "amblight");
	if (loc != -1)
		glUniform4fv( loc, 1, (float*)&( ambLight ) );

	loc = glGetUniformLocation(shaderProgram, "diffpos");
	if (loc != -1)
		glUniform4fv( loc, 1, (float*)&( diffusePosition ) );

	loc = glGetUniformLocation(shaderProgram, "difflight");
	if (loc != -1)
		glUniform4fv( loc, 1, (float*)&( diffuseColor ) );

	loc = glGetUniformLocation(shaderProgram, "specColor");
	if (loc != -1)
		glUniform4fv( loc, 1, (float*)&( specColor ) );

	loc = glGetUniformLocation(shaderProgram, "tightness");
	if (loc != -1)
		glUniform1f( loc, tightness );

	loc = glGetUniformLocation(shaderProgram, "eye");
	if (loc != -1)
		glUniform4fv( loc, 1, (float*)&( cameraOne.getFrom() ) );

	loc = glGetUniformLocation(shaderProgram, "useLighting");
	if (loc != -1)
		glUniform1f( loc, useLighting );

	loc = glGetUniformLocation(shaderProgram, "colorInfluence");
	if (loc != -1)
		glUniform4fv( loc, 1, (float*)&(objColor) );

	glUseProgram( passProgram );
	loc = glGetUniformLocation(passProgram, "world");
	if (loc != -1)
		glUniformMatrix4fv( loc, 1, false, (float*)&( glm::translate(glm::mat4(),diffusePosition) * glm::scale(vec3(0.25,0.25,0.25)) ) );
	cameraOne.updateModelViewProjection( passProgram, "modelViewProjection" );
	cube.draw();
	glUseProgram( shaderProgram );
	


	//hill.draw(shaderProgram);
	/*if (loc != -1)
		glUniform3fv( loc, 1, (float*)&vec3( 0, 0, 1.0f ) );*/

	ter.draw( shaderProgram );
	for( int i = 0; i < 100; i++ )
	{
		decorations[i].draw( shaderProgram );
	}
}

void OpenGlWindow::paintPort( Player &currentPlayer, Camera &currentCamera )
{
	currentCamera.setAspect( width()/ height() );

	GLfloat playerDir = currentPlayer.getDirection();
	vec3 playerPos = currentPlayer.getPosition();

	//Finds a place for the camera that doesn't intersect terrain
	vec3 nextFrom = playerPos + vec3( -cos(playerDir)*3.0f, 2.0f, -sin(playerDir)*3.0f )*currentPlayer.getScale();
	GLfloat terHeight = ter.fitPosition( nextFrom ).y + 0.4f;
	if ( terHeight > nextFrom.y )
		nextFrom.y = terHeight;

	//Makes a whooshing camera effect
	GLfloat delay = 15.0f;
	currentCamera.setFrom( (currentCamera.getFrom() * delay + nextFrom ) /(delay+1) );
	currentCamera.setTo( 
		//hill.getPosition() );
		playerPos + vec3( 0, 0.5f, 0 ) );

	currentCamera.updateModelViewProjection( shaderProgram, "modelViewProjection" );

	GLint loc = glGetUniformLocation(shaderProgram, "drawMode");
	if (loc != -1)
	{
		GLint mode = 0;
		if ( GetAsyncKeyState( 'Z' ) )
		{
			mode = 1;
		}
		else
		if ( GetAsyncKeyState( 'X' ) )
		{
			mode = 2;
		}
		else
		if ( GetAsyncKeyState( 'C' ) )
		{
			mode = 3;
		}

		glUniform1i( loc, mode );
	}

	loc = glGetUniformLocation(shaderProgram, "light");
	if (loc != -1)
	{
		//std::cout << "light";
		glUniform4fv( loc, 1, (float*)&( glm::vec4( currentCamera.getFrom(), 0 ) + glm::vec4( 0, 3.0f, 0, 1 ) ) );
	}


	loc = glGetUniformLocation(shaderProgram, "colorInfluence");
	if (loc != -1)
		glUniform3fv( loc, 1, (float*)&vec3( 0, 0, 0 ) );
	hill.draw(shaderProgram);
	if (loc != -1)
		glUniform3fv( loc, 1, (float*)&vec3( 0, 0, 1.0f ) );
	playerOne.draw( shaderProgram );
	if (loc != -1)
		glUniform3fv( loc, 1, (float*)&vec3( 1.0f, 0, 0 ) );
	playerTwo.draw( shaderProgram );
	if (loc != -1)
		glUniform3fv( loc, 1, (float*)&vec3( 0, 0, 0 ) );
	ter.draw( shaderProgram );
	for( int i = 0; i < 100; i++ )
	{
		decorations[i].draw( shaderProgram );
	}
}