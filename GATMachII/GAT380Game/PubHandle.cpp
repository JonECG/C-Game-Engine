#include "PubHandle.h"

#include "Renderer\Shader.h"
#include "Window\ContentManager.h"
#include "Window\Graphics.h"
#include "Window\Font.h"
#include "Window\Character.h"

#include "Window\UserController.h"
#include "Window\CoordinateTransform.h"

#include "glm\glm.hpp"
#include "ShapeGenerator.h"

#include "Game.h"
#include "Stage.h"
#include "Entity.h"

#include "LevelControllerComponent.h"

Texture *cow;
Font *font;
Game * game;

glm::vec3 lightPos = glm::vec3( 1, 2, 10 );
glm::vec4 difLight = glm::vec4(1.0f,1.0f,1.0f,1), colInfluence = glm::vec4(0.8f,0.8f,0.8f,1), specColor = glm::vec4(0.8f,0.8f,0.8f,1);
float specAmount = 10;



void PubHandle::init( ContentManager* content, CoordinateTransform *transform, Graphics * graphics )
{
	transform->setCoordinateSystem( CoordinateSystem::OPENGL_COORDINATES );

	graphics->showFullscreen();

	cow = content->loadPreppedTexture( "Assets/cow.glt" );
	font = content->loadFont( "Assets/Tahoma.bfnt" );
	//Shader * passShad = content->loadShader( "Assets/passThrough.vert", "Assets/passThrough.frag" );
	Shader * planeShad = content->loadShader( "Assets/plane.vert", "Assets/plane.frag" );
	Shader * textureShad = content->loadShader( "Assets/texture.vert", "Assets/texture.frag" );

	game = new Game();
	game->subscribeContentManager( content );
	game->subscribeGraphicsHandle( graphics );
	
	Stage * stage = new Stage();
	game->setStage( stage );

	Geometry * planeGeo = content->loadGeometry( "Assets/biplane.glmod" );
	Geometry * cubeGeo = ShapeGenerator::createCube( content );

	Entity * controller = new Entity();
	LevelControllerComponent * cont = new LevelControllerComponent();
	cont->friendlyPlane = planeGeo->makeRenderable( planeShad, content->loadPreppedTexture( "Assets/planeTex.glt" ) );
	cont->enemyPlane = planeGeo->makeRenderable( planeShad, content->loadPreppedTexture( "Assets/planeTex.glt" ) );
	cont->bullet = content->loadGeometry( "Assets/shovel.glmod" )->makeRenderable(textureShad,content->loadPreppedTexture( "Assets/ShovelTexture.glt" ));
	cont->ground = cubeGeo->makeRenderable( textureShad, content->loadPreppedTexture( "Assets/brickyDiffuse.glt" ) );
	cont->indicator = content->loadGeometry( "Assets/arrow.glmod" )->makeRenderable(textureShad);//, content->loadPreppedTexture( "Assets/ShovelTexture.glt" ));

	controller->addComponent( cont );
	stage->addEntity( controller );
	
	glm::vec4 amb = glm::vec4(0.1f,0.1f,0.1f,1);
	Shader::setGlobalUniformParameter( "amblight", PT_VEC4, &amb );//set
	Shader::addGlobalUniformParameter( "diffpos", PT_VEC3, &lightPos );
	Shader::addGlobalUniformParameter( "difflight", PT_VEC4, &difLight);//set
	Shader::addGlobalUniformParameter( "colorInfluence", PT_VEC4, &colInfluence);//set
	Shader::addGlobalUniformParameter( "specColor", PT_VEC4, &specColor);//set
	Shader::addGlobalUniformParameter( "tightness", PT_FLOAT, &(specAmount) );
}

void PubHandle::shutdown()
{
	delete game;
}

void PubHandle::paint( Graphics* graphics )
{
	graphics->setClearColor( 0, 0.7f, 1, 1 );
	game->subscribeGraphicsHandle( graphics );
	game->draw();
}

void PubHandle::update( float dt, const UserController *uc )
{
	game->subscribeUserController( uc );
	game->update( dt );
}
