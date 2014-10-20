#include "GraphicsHandle.h"
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\ext.hpp>
#include "Camera.h"
#include <QtGui\QMouseEvent>
#include "ShapeGenerator.h"

#include "DebugMenus.h"
#include <Qt\qapplication.h>
#include "Stage.h"
#include "Entity.h"

#include "CameraComponent.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "FollowCamComponent.h"
#include "InputMoveComponent.h"
GeneralGlWindow::ShaderInfo *passShad;

GeneralGlWindow::TextureInfo *perlinMap;
GeneralGlWindow::Renderable *perlinShow;

glm::vec3 lightPos = glm::vec3( 1, 2, 10 );
glm::vec4 difLight = glm::vec4(1.0f,1.0f,1.0f,1), colInfluence = glm::vec4(0.8f,0.8f,0.8f,1), specColor = glm::vec4(0.8f,0.8f,0.8f,1);
float specAmount = 10;

void GraphicsHandle::init()
{
	passShad = addShaderInfo( "res/passThrough.vert", "res/passThrough.frag" );
	textShad = addShaderInfo( "res/texture.vert", "res/texture.frag" );
	
	Neumont::ShapeData sphereData = Neumont::ShapeGenerator::makeSphere( 10 );
	for( int i = 0; i < sphereData.numVerts; i++ )
	{
		Neumont::Vertex * vert = &sphereData.verts[i];
		vert->uv = glm::vec2( ( std::atan2( vert->position.y, vert->position.x ) ) / 6.28 + 0.5, 0.5 - std::asin( vert->position.z ) / 3.14 );
	}
	GeneralGlWindow::GeometryInfo * sphereGeo = addGeometry( sphereData.verts, sphereData.numVerts, sphereData.indices, sphereData.numIndices, GL_TRIANGLES );

	setUpAttribs( sphereGeo );
	
	GeneralGlWindow::Renderable * sphere = addRenderable( sphereGeo, glm::mat4(), passShad );

	Stage * stage = new Stage();
	Stage::setMainStage( stage );

	Entity * cam = new Entity();
	cam->addComponent( new TransformComponent() );
	cam->addComponent( new CameraComponent() );
	cam->addComponent( new FollowCamComponent() );
	cam->gc<TransformComponent>()->setTranslation( glm::vec3( -5, 0, 0 ) );
	stage->addEntity(cam);

	Entity * sphereEnt = new Entity();
	sphereEnt->addComponent( new TransformComponent() );
	sphereEnt->addComponent( new RenderComponent() );
	sphereEnt->addComponent( new InputMoveComponent() );
	sphereEnt->gc<RenderComponent>()->setRenderable( sphere );
	stage->addEntity(sphereEnt);

	cam->gc<FollowCamComponent>()->target = sphereEnt;
	cam->gc<FollowCamComponent>()->desiredOffset = glm::vec3( -10, 0, 0 );

	Entity * sphereEnt2 = new Entity();
	sphereEnt2->addComponent( new TransformComponent() );
	sphereEnt2->addComponent( new RenderComponent() );
	sphereEnt2->gc<RenderComponent>()->setRenderable( addRenderable( sphereGeo, glm::mat4(), passShad ) );
	stage->addEntity(sphereEnt2);
	sphereEnt2->getComponent<TransformComponent>()->setTranslation( glm::vec3( 0, -2, 0 ) );
	
	setUniformParameter( "amblight", PT_VEC4, (float*)&glm::vec4(0.1f,0.1f,0.1f,1) );//set
	addUniformParameter( "diffpos", PT_VEC3, (float*)&lightPos );
	addUniformParameter( "difflight", PT_VEC4, (float*)&difLight);//set
	addUniformParameter( "colorInfluence", PT_VEC4, (float*)&colInfluence);//set
	addUniformParameter( "specColor", PT_VEC4, (float*)&specColor);//set
	addUniformParameter( "tightness", PT_FLOAT, (float*)&(specAmount) );
}


void GraphicsHandle::paint()
{	
	glViewport( 0, 0, width(), height() );
	paintInner();
}

void GraphicsHandle::paintInner()
{
	Stage::getMainStage()->draw(this);
}

void GraphicsHandle::update( float dt )
{
	Stage::getMainStage()->update(dt);
	repaint();
}