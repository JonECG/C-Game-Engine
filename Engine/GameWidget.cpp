#include "GameWidget.h"
#include <ShapeGenerator.h>
#include <iostream>
#include <glm\glm.hpp>
#include <glm\ext.hpp>
#include "Camera.h"
#include <QtGui\QMouseEvent>

GeneralGlWindow::Renderable ** renderableTests;

float random()
{
	return std::rand() / (float)RAND_MAX;
}

glm::mat4 randomRotation()
{
	return glm::rotate( glm::mat4(), random() * 180, glm::normalize( glm::vec3( random()*2-1,random()*2-1,random()*2-1 ) ) );
}

Camera camera;

void GameWidget::init()
{
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_DEPTH_TEST );

	ShaderInfo* textShad = addShaderInfo( "res/texture.vert", "res/texture.frag" );

	Neumont::ShapeData teapot = Neumont::ShapeGenerator::makeTeapot( 5, glm::mat4() );

	TextureInfo* brick = addTexture( "res/brick.png" );
	TextureInfo* mask = addTexture( "res/Frigate.png" );

	
	GeometryInfo* maskGeo = loadFile( "res/Frigate.mod" );
	setUpAttribs( maskGeo );
	GeometryInfo* cubeGeo = loadFile( "res/cube.mod" );
	setUpAttribs( cubeGeo );
	GeometryInfo* teapotGeo = addGeometry( teapot.verts, teapot.numVerts, teapot.indices, teapot.numIndices, GL_TRIANGLES );
	setUpAttribs( teapotGeo );

	camera = Camera( float(width())/height(), 0.1, 100 );
	camera.setFrom( glm::vec3( 0,0,4) );
	camera.setTo( glm::vec3(0,0,0) );

	renderableTests = new Renderable*[3];
	renderableTests[0] = addRenderable( maskGeo, glm::scale( glm::vec3( 0.02f ) ), textShad, mask );
	renderableTests[1] = addRenderable( cubeGeo, glm::translate( glm::vec3( -0.3, -0.4, 0 ) ) * glm::scale(glm::vec3(0.15)) , textShad, brick );


	addUniformParameter( textShad, "mvp", PT_MAT4, (float*)&camera.mvp );

	float tightness = 40.0f;
	setUniformParameter( textShad, "amblight", PT_VEC4, (float*)&glm::vec4(0.1f,0.1f,0.1f,1) );
	setUniformParameter( textShad, "diffpos", PT_VEC4, (float*)&glm::vec4(3.0f,3.0f,0.0f,1) );
	setUniformParameter( textShad, "difflight", PT_VEC4, (float*)&glm::vec4(0.8f,0.8f,0.8f,1));
	setUniformParameter( textShad, "specColor", PT_VEC4, (float*)&glm::vec4(0.8f,0.8f,0.8f,1));
	setUniformParameter( textShad, "tightness", PT_FLOAT, (float*)&(tightness) );
	addUniformParameter( textShad, "eye", PT_VEC4, (float*)&camera.from);
}

float angle = 0;


void GameWidget::paint()
{
	camera.setAspect( float(width())/height() );
	glViewport( 0, 0, width(), height() );
	glClearColor( 0.1, 0.1, 0.1, 0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	for( int i = 0; i < 2; i++ )
	{
		renderableTests[i]->draw();
	}
}

glm::vec2 mousePosition = glm::vec2();
bool mouseTracking = false;

void GameWidget::mousePressEvent( QMouseEvent * mEvent )
{
	if( !mouseTracking )
	{
		mousePosition.x = mEvent->x()/float(width());
		mousePosition.y = mEvent->y()/float(height());
	}
	mouseTracking = true;
}

void GameWidget::mouseReleaseEvent( QMouseEvent * mEvent )
{
	mouseTracking = false;
}

void GameWidget::mouseMoveEvent( QMouseEvent * mEvent )
{
	if ( mouseTracking )
	{
		glm::vec2 currentPosition = glm::vec2( mEvent->x()/float(width()), mEvent->y()/float(height()));

		glm::vec2 delta = currentPosition-mousePosition;

		glm::vec3 camNorm = glm::normalize( camera.getTo()-camera.getFrom() );

		glm::vec3 rotUp = glm::cross( glm::vec3(0,1,0), camNorm );
		glm::vec3 rotRight = glm::cross( rotUp, camNorm );


		camera.setTo( camera.getFrom() + glm::rotate( glm::rotate( camNorm, delta.y*100, rotUp ), delta.x*100, rotRight ) );
		//camera.setTo( camera.getFrom() + glm::rotate( camNorm, delta.y*100, rotUp ) );
		//camera.setTo( glm::vec3( mousePosition.x, mousePosition.y, 0 ) );

		mousePosition.x = currentPosition.x;
		mousePosition.y = currentPosition.y;
	}
}

void GameWidget::update( float dt )
{
	static glm::vec2 mousePosition = glm::vec2();

	glm::vec3 camNorm = glm::normalize( camera.getTo()-camera.getFrom() );
	glm::vec3 strafe = glm::normalize( -glm::cross( glm::vec3(0,1,0), camNorm ) );
	glm::vec3 up = glm::normalize( glm::cross( strafe, camNorm ) );

	glm::mat3 camSpace = glm::mat3( strafe, camNorm, up );

	glm::vec3 movement;
	/*if ( GetAsyncKeyState(VK_SHIFT) )
		movement = glm::vec3( (GetAsyncKeyState('D')?1:0) - (GetAsyncKeyState('A')?1:0), 0, (GetAsyncKeyState('W')?1:0) - (GetAsyncKeyState('S')?1:0) );
	else*/
		movement = glm::vec3( (GetAsyncKeyState('D')?1:0) - (GetAsyncKeyState('A')?1:0), (GetAsyncKeyState(VK_SPACE)?1:0) - (GetAsyncKeyState(VK_SHIFT)?1:0),(GetAsyncKeyState('W')?1:0) - (GetAsyncKeyState('S')?1:0) );

	glm::vec3 worldMove = camSpace*movement * 1.0f * dt;

	camera.setTo( camera.getTo() + worldMove);
	camera.setFrom( camera.getFrom() + worldMove);

	camera.calcModelViewProjection();

	//angle++;
	repaint();
}