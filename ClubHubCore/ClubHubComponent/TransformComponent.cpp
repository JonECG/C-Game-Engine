#include "TransformComponent.h"
#include "glm\ext.hpp"

TransformComponent::TransformComponent()
{
	scaling = glm::vec3(1,1,1);
}

void TransformComponent::translate( glm::vec3 offset )
{
	position += offset;
}
glm::vec3 TransformComponent::getTranslation()
{
	return position;
}
void TransformComponent::setTranslation( glm::vec3 position )
{
	this->position = position;
}
glm::mat4 TransformComponent::getTranslationMatrix()
{
	return glm::translate( position );
}

void TransformComponent::scale( glm::vec3 scale )
{
	this->scaling *= scale;
}
void TransformComponent::setScale( glm::vec3 scale )
{
	this->scaling = scale;
}
glm::vec3 TransformComponent::getScale()
{
	return this->scaling;
}
glm::mat4 TransformComponent::getScaleMatrix()
{
	return glm::scale( scaling );
}

void TransformComponent::rotate( glm::vec3 axis, float angle )
{
	rotation *= glm::rotate( angle, axis );
}
void TransformComponent::setRotation( glm::vec3 axis, float angle )
{
	rotation = glm::rotate( angle, axis );
}
void TransformComponent::setRotation( glm::mat3 rot )
{
	rotation = glm::mat4( rot );
}
glm::mat4 TransformComponent::getRotationMatrix()
{
	return rotation;
}

glm::mat4 TransformComponent::getLinearTransformation()
{
	return getRotationMatrix() * getScaleMatrix();
}
glm::mat4 TransformComponent::getTransformation()
{
	return getTranslationMatrix() * getLinearTransformation();
}