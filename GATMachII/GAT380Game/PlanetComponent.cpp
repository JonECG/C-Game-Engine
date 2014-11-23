#include "PlanetComponent.h"
#include "Entity.h"
#include "Components\RenderComponent.h"
#include "Components\TransformComponent.h"
#include "Stage.h"
#include "Game.h"
#include "Window\ContentManager.h"
#include "ShapeGenerator.h"

#include "noise\noise.h"
#include "Renderer\Texture.h"

#include "Renderer\Shader.h"
#include <glm\ext.hpp>

PlanetComponent::PlanetComponent()
{
	noiseMap = 0;
	currentRot = 0;
	orbitRate = 1;
	twinkleTime = 0;
}

void PlanetComponent::init()
{
	noise::module::Perlin perlinModule;
	perlinModule.SetFrequency( 10 );
	perlinModule.SetPersistence( 0.5f );
	perlinModule.SetOctaveCount( (int) 11 );
	perlinModule.SetLacunarity( 3.0f );
	perlinModule.SetSeed( 22 );

	int dimensions = 1024;

	if( noiseMap == 0 )
	{
		noiseMap = Texture::createTexture(dimensions,dimensions);

		byte * data = new byte[ 4*dimensions * dimensions ];
		//*img = QImage( dimensions, dimensions, QImage::Format::Format_ARGB32 );
		for( int x = 0; x < dimensions; x++ )
		{
			for( int y = 0; y < dimensions; y++ )
			{
				double value = ( perlinModule.GetValue( x/(float)(dimensions), y/(float)(dimensions), 0/10 ) + 1 ) / 2;
				value = (value > 1) ? 1 : value;
				value = (value < 0) ? 0 : value;
				byte amount = (byte) (255 * value );
				data[ 4*(y * dimensions + x) + 0 ] = amount;
				data[ 4*(y * dimensions + x) + 1 ] = amount;
				data[ 4*(y * dimensions + x) + 2 ] = amount;
				data[ 4*(y * dimensions + x) + 3 ] = 255;
			}
		}
		noiseMap->updateTexture( dimensions, dimensions, data );

		delete data;
	}

	auto rend = getParent()->gc<RenderComponent>();
	auto cont =  getParent()->getStage()->getGame()->getContentManager();
	Geometry * sphereGeo = ShapeGenerator::createSphere( cont, 100 );
	rend->setRenderable( sphereGeo->makeRenderable( cont->loadShader( "Assets/perlin.vert", "Assets/perlin.frag" ), noiseMap, true ) );
	getParent()->gc<TransformComponent>()->setScale( glm::vec3( 1 ) );
	getParent()->gc<TransformComponent>()->setRotation( glm::mat3( glm::rotate( glm::mat4(), 90.0f, glm::vec3( 1,0, 0 ) ) * glm::rotate( glm::mat4(), 90.0f, glm::vec3( 0, 0, 1 ) ) ) );

	Entity * starrySky = new Entity;
	starrySky->addComponent( new RenderComponent() );
	starrySky->addComponent( new TransformComponent() );
	starrySky->gc<RenderComponent>()->setRenderable( sphereGeo->makeRenderable( cont->loadShader( "Assets/texture.vert", "Assets/starLit.frag" ), noiseMap, true ) );
	starrySky->gc<TransformComponent>()->setScale( glm::vec3( -10 ) );
	starrySky->gc<TransformComponent>()->setRotation( glm::mat3( glm::rotate( glm::mat4(), 90.0f, glm::vec3( 1,0, 0 ) ) * glm::rotate( glm::mat4(), 90.0f, glm::vec3( 0, 0, 1 ) ) ) );
	getParent()->getStage()->addEntity( starrySky );

	sunEntity = new Entity;
	sunEntity->addComponent( new RenderComponent() );
	sunEntity->addComponent( new TransformComponent() );
	sunEntity->gc<RenderComponent>()->setRenderable( sphereGeo->makeRenderable( cont->loadShader( "Assets/passThrough.vert", "Assets/passThrough.frag" ), cont->loadPreppedTexture( "Assets/sun.glt" ), true ) );
	getParent()->getStage()->addEntity( sunEntity );
}

void PlanetComponent::update( float dt )
{
	twinkleTime += dt;
	currentRot += orbitRate * dt;
	glm::vec4 lightPos = glm::vec4( sin( currentRot )*9, 0, cos(currentRot)*9, 1 );
	sunEntity->gc<TransformComponent>()->setTranslation( glm::vec3( lightPos ) );
	sunEntity->gc<TransformComponent>()->setScale(  glm::vec3( 1 ) );

	/*
	uniform vec4 amblight;
	uniform vec4 diffpos;
	uniform vec4 difflight;
	uniform vec4 specColor;
	uniform float tightness;
	uniform vec4 eye;
	uniform float timey;
	*/

	Shader::setGlobalUniformParameter( "timey", ParameterType::PT_FLOAT, &twinkleTime );
	Shader::setGlobalUniformParameter( "diffpos", ParameterType::PT_VEC4, &lightPos );

	glm::vec4 amb( 0.1f, 0.1f, 0.1f, 1.0f );
	Shader::setGlobalUniformParameter( "amblight", ParameterType::PT_VEC4, &amb );

	glm::vec4 dl( 1.0f, 1.0f, 1.0f, 1.0f );
	Shader::setGlobalUniformParameter( "difflight", ParameterType::PT_VEC4, &dl );
	Shader::setGlobalUniformParameter( "specColor", ParameterType::PT_VEC4, &dl );

	float tight = 3;
	Shader::setGlobalUniformParameter( "tightness", ParameterType::PT_FLOAT, &tight );
}