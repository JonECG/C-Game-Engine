#include "CameraComponent.h"
#include "TransformComponent.h"
#include "Entity.h"
#include "GeneralGLWindow.h"
#include "Stage.h"
#include "GeneralGLWindow.h"
#include <glm\ext.hpp>
#include <GL\glew.h>

CameraComponent::CameraComponent()
{
	aspect = 1;
	camNear = 0.01;
	camFar = 200;
	fov = 50;
}

void CameraComponent::lookAt( glm::vec3 to, glm::vec3 up )
{
	/*TransformComponent * trans = parent->getComponent<TransformComponent>();
	if( trans != nullptr )
	{
		trans->transformation = glm::loew
	}*/
}

void CameraComponent::update( float dt )
{
	Stage::getMainStage()->subscribeRender( this );
}

void CameraComponent::sendCamInformation( GeneralGlWindow * window )
{
	glm::vec3 from = glm::vec3();
	glm::vec3 to = glm::vec3(1,0,0);
	glm::vec3 up = glm::vec3(0,1,0);
	TransformComponent * trans = parent->getComponent<TransformComponent>();
	if( trans != nullptr )
	{
		from = glm::vec3( trans->getTransformation() * glm::vec4( from, 1 ) );
		to = glm::vec3( trans->getTransformation() * glm::vec4( to, 1 ) );
		up = glm::vec3( trans->getRotationMatrix() * glm::vec4( up, 1 ) );
	}
	glm::mat4 camcam = glm::perspective(fov, aspect, camNear, camFar) * glm::lookAt( from, to, up );
	window->setUniformParameter( "mvp", GeneralGlWindow::PT_MAT4, (float*)&camcam );
}