#include "CameraComponent.h"
#include "TransformComponent.h"
#include "Entity.h"
#include "Stage.h"
#include <glm\ext.hpp>
#include "Renderer\Shader.h"
#include "Game.h"
#include "Window\Graphics.h"
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
	//glm::mat4 mat = glm::lookAt( parent->getComponent<TransformComponent>()->getTranslation(), to, up );
	//parent->getComponent<TransformComponent>()->setRotation( glm::mat3( mat ) );

	glm::vec3 from = parent->getComponent<TransformComponent>()->getTranslation();

	glm::vec3 zaxis = -glm::normalize(to - from);
	glm::vec3 xaxis = glm::normalize(glm::cross(up, zaxis));
	glm::vec3 yaxis = glm::cross(zaxis, xaxis);

	glm::mat3 lookMat = glm::mat3( xaxis, yaxis, zaxis );

	parent->getComponent<TransformComponent>()->setRotation( lookMat );
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


int CameraComponent::getNumberOfRuns()
{
	return 1;
}

void CameraComponent::setUp()
{
}

void CameraComponent::startRun( int run )
{
	run;
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

void CameraComponent::endRun( int run )
{
	run;
}

void CameraComponent::breakDown()
{
}