#include "GraphicsHandle.h"
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\ext.hpp>
#include "Camera.h"
#include <QtGui\QMouseEvent>
#include "ShapeGenerator.h"

#include "DebugMenus.h"


Camera camera;
float tightness;
glm::vec3 shipPos;
float fps, delta;
float timey;

const char * stringy = "This is a string";

GeneralGlWindow::ShaderInfo *questionShad;
GeneralGlWindow::Renderable * character, *bushRend, *groundRend, *skyRend, *questionBlock;

void GraphicsHandle::init()
{
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_DEPTH_TEST );

	//DebugMenus::inject( layout() );
	//DebugMenus::menu->show();

	textShad = addShaderInfo( "res/texture.vert", "res/texture.frag" );
	questionShad = addShaderInfo( "res/texture.vert", "res/question.frag" );

	GeneralGlWindow::TextureInfo * marioAndWeegee = addTexture( "res/marioAndLuigi.png" );
	GeneralGlWindow::TextureInfo * marioAndWeegeeTrans = addTexture( "res/marioAndLuigiTrans.png" );

	GeneralGlWindow::TextureInfo * bush = addTexture( "res/bush.png" );
	GeneralGlWindow::TextureInfo * bushTrans = addTexture( "res/bushTrans.png" );

	GeneralGlWindow::TextureInfo * ground = addTexture( "res/grass.png" );
	GeneralGlWindow::TextureInfo * sky = addTexture( "res/sky.png" );

	GeneralGlWindow::TextureInfo * question = addTexture( "res/question.png" );
	GeneralGlWindow::TextureInfo * noise = addTexture( "res/seamless.png" );

	Neumont::ShapeData charData = Neumont::ShapeGenerator::makePlane(2);
	GeneralGlWindow::GeometryInfo * charGeo = addGeometry( charData.verts, charData.numVerts, charData.indices, charData.numIndices, GL_TRIANGLES );

	Neumont::ShapeData cubeData = Neumont::ShapeGenerator::makeCube();
	GeneralGlWindow::GeometryInfo * cubeGeo = addGeometry( cubeData.verts, cubeData.numVerts, cubeData.indices, cubeData.numIndices, GL_TRIANGLES );
	
	setUpAttribs( charGeo );
	setUpAttribs( cubeGeo );
	character = addRenderable( charGeo, glm::translate( glm::vec3( -2, 0, 2 ) )*glm::rotate( 90.0f, glm::vec3( 1,0,0 ) ) * glm::scale( glm::vec3( 1, 1 , -1 ) ), textShad, marioAndWeegee, marioAndWeegeeTrans );
	bushRend = addRenderable( charGeo, glm::translate( glm::vec3( -3, 0, 1 ) )*glm::rotate( 90.0f, glm::vec3( 1,0,0 ) ) * glm::scale( glm::vec3( 1, 1 , -1 ) ), textShad, bush, bushTrans );
	groundRend = addRenderable( charGeo, glm::translate( glm::vec3( 0, -1, 4 ) )*glm::scale( glm::vec3( 4.0f ) ), textShad, ground );
	skyRend = addRenderable( charGeo, glm::translate( glm::vec3( 0, 3, 0 ) )*glm::scale( glm::vec3( 4.0f ) )*glm::rotate( 90.0f, glm::vec3( 1,0,0 ) ) * glm::scale( glm::vec3( 1, 1 , -1 ) ), textShad, sky );

	questionBlock = addRenderable( cubeGeo, glm::translate( glm::vec3( -1, 0.5f, 2 ) )*glm::scale( glm::vec3( 0.2f ) ), questionShad, question, noise );
	/*character->trans = flagByTexture;*/


	tightness = 40.0f;

	camera = Camera( float(width())/height(), 0.1, 100 );
	camera.setFrom( glm::vec3( -1, 0.5f, 5 ) );
	camera.setTo( glm::vec3(-2,0,0) );

	addUniformParameter( textShad, "mvp", PT_MAT4, (float*)&camera.mvp );
	
	setUniformParameter( textShad, "amblight", PT_VEC4, (float*)&glm::vec4(0.1f,0.1f,0.1f,1) );
	setUniformParameter( textShad, "diffpos", PT_VEC4, (float*)&glm::vec4(3.0f,6.0f,8.0f,1) );
	setUniformParameter( textShad, "difflight", PT_VEC4, (float*)&glm::vec4(0.8f,0.8f,0.8f,1));
	setUniformParameter( textShad, "specColor", PT_VEC4, (float*)&glm::vec4(0.8f,0.8f,0.8f,1));
	addUniformParameter( textShad, "tightness", PT_FLOAT, (float*)&(tightness) );
	addUniformParameter( textShad, "eye", PT_VEC4, (float*)&camera.from);

	addUniformParameter( questionShad, "mvp", PT_MAT4, (float*)&camera.mvp );
	
	setUniformParameter( questionShad, "amblight", PT_VEC4, (float*)&glm::vec4(0.1f,0.1f,0.1f,1) );
	setUniformParameter( questionShad, "diffpos", PT_VEC4, (float*)&glm::vec4(3.0f,6.0f,8.0f,1) );
	setUniformParameter( questionShad, "difflight", PT_VEC4, (float*)&glm::vec4(0.8f,0.8f,0.8f,1));
	setUniformParameter( questionShad, "specColor", PT_VEC4, (float*)&glm::vec4(0.8f,0.8f,0.8f,1));
	addUniformParameter( questionShad, "tightness", PT_FLOAT, (float*)&(tightness) );
	addUniformParameter( questionShad, "eye", PT_VEC4, (float*)&camera.from);

	addUniformParameter( questionShad, "timey", PT_FLOAT, &timey);

	DebugMenus::watchFloat( "Time1", timey );
	DebugMenus::watchFloat( "Time2", timey );
	DebugMenus::watchFloat( "Time3", timey, "Not default" );
	DebugMenus::watchFloat( "Time4", timey, "Not default" );
	DebugMenus::watchText( "Some Text", stringy, "Not default" );
	DebugMenus::watchFloat( "Time5", timey );
}

float angle = 0;

void GraphicsHandle::paint()
{
	camera.setAspect( float(width())/height() );
	glViewport( 0, 0, width(), height() );
	glClearColor( 0.5, 0.1, 0.1, 0 );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	character->draw();
	bushRend->draw();
	groundRend->draw();
	skyRend->draw();
	questionBlock->draw();
}

glm::vec2 mousePosition = glm::vec2();
bool mouseTracking = false;


void GraphicsHandle::mousePressEvent( QMouseEvent * mEvent )
{
	if ( mEvent->button() == Qt::MouseButton::RightButton )
	{

		if( !mouseTracking )
		{
			mousePosition.x = mEvent->x()/float(width());
			mousePosition.y = mEvent->y()/float(height());
		}
		mouseTracking = true; 
	}

}

void GraphicsHandle::mouseReleaseEvent( QMouseEvent * mEvent )
{
	if ( mEvent->button() == Qt::MouseButton::RightButton )
	{
		mouseTracking = false;
	}
}

void GraphicsHandle::mouseMoveEvent( QMouseEvent * mEvent )
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

void GraphicsHandle::update( float dt )
{
	static glm::vec2 mousePosition = glm::vec2();

	timey += dt;

	fps = 1/dt;
	delta = dt;

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

	repaint();
}