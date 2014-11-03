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

	cow = content->loadTexture( "Resources/cow.png" );
	font = content->loadFont( "Resources/Tahoma.bfnt" );
	Shader * passShad = content->loadShader( "Resources/passThrough.vert", "Resources/passThrough.frag" );

	game = new Game();
	game->subscribeContentManager( content );
	game->subscribeGraphicsHandle( graphics );
	
	Stage * stage = new Stage();
	game->setStage( stage );

	Geometry * planeGeo = content->loadGeometry( "Resources/biplane.mod" );
	Geometry * cubeGeo = ShapeGenerator::createCube( content );

	Entity * controller = new Entity();
	LevelControllerComponent * cont = new LevelControllerComponent();
	cont->friendlyPlane = planeGeo->makeRenderable( passShad, content->loadTexture( "Resources/planeTex.png" ) );
	cont->enemyPlane = planeGeo->makeRenderable( passShad, content->loadTexture( "Resources/planeTex.png" ) );
	cont->bullet = content->loadGeometry( "Resources/shovel.mod" )->makeRenderable(passShad,content->loadTexture( "Resources/ShovelTexture.png" ));
	cont->ground = cubeGeo->makeRenderable( passShad, content->loadTexture( "Resources/brickyDiffuse.png" ) );

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
