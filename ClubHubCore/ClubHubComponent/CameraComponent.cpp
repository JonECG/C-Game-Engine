#include "CameraComponent.h"
#include "TransformComponent.h"
#include "Entity.h"
#include "Stage.h"
#include <glm\ext.hpp>
#include "Shader.h"
#include "Game.h"
#include "Graphics.h"
#include <glm\glm.hpp>

CameraComponent::CameraComponent()
{
	aspect = 1;
	camNear = 0.01f;
	camFar = 200;
	fov = 50;
}

void CameraComponent::lookAt( glm::vec3 to, glm::vec3 up )
{
	to;
	up;

	glm::mat4 mat = glm::lookAt( parent->getComponent<TransformComponent>()->getTranslation(), to, up );
	//parent->getComponent<TransformComponent>()->setRotation( glm::mat3( mat ) );
	/*TransformComponent * trans = parent->getComponent<TransformComponent>();
	if( trans != nullptr )
	{
		trans->transformation = glm::loew
	}*/
}

void CameraComponent::update( float dt )
{
	dt;
	parent->getStage()->subscribeRender( this );
}

void CameraComponent::draw()
{
	//Graphics * g = parent->getStage()->getGame()->getGraphicsHandle();
	//g->setTransform( glm::mat4() );
	//g->drawText( -0.5f, -0.5f, "Tried to draw text", 0.2f );
}

void CameraComponent::sendCamInformation()
{
	glm::vec3 from = glm::vec3();
	glm::vec3 to = glm::vec3(0,0,-1);
	glm::vec3 up = glm::vec3(0,1,0);
	TransformComponent * trans = parent->getComponent<TransformComponent>();
	if( trans != nullptr )
	{
		from = glm::vec3( trans->getTransformation() * glm::vec4( from, 1 ) );
		to = glm::vec3( trans->getTransformation() * glm::vec4( to, 1 ) );
		up = glm::vec3( trans->getRotationMatrix() * glm::vec4( up, 1 ) );
	}
	//std::cout << "<" << from.x << ", " << from.y << ", " << from.z << "> <" << to.x << ", " << to.y << ", " << to.z << "> <" << up.x << ", " << up.y << ", " << up.z << ">" << std::endl;
	glm::mat4 camcam = glm::perspective(fov, aspect, camNear, camFar) * glm::lookAt( from, to, up );
	Shader::setGlobalUniformParameter( "mvp", ParameterType::PT_MAT4, &camcam );
}