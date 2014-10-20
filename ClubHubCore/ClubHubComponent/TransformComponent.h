#pragma once
#include "glm\glm.hpp"
#include "Component.h"
#include "definition.h"

class EXPORT TransformComponent : public Component
{
	glm::vec3 position;
	glm::vec3 scaling;
	glm::mat4 rotation;
	glm::mat4 transformation;
public:
	TransformComponent();

	void translate( glm::vec3 offset );
	glm::vec3 getTranslation();
	void setTranslation( glm::vec3 position );
	glm::mat4 getTranslationMatrix();

	void scale( glm::vec3 scale );
	void setScale( glm::vec3 scale );
	glm::vec3 getScale();
	glm::mat4 getScaleMatrix();

	void rotate( glm::vec3 axis, float angle );
	void setRotation( glm::vec3 axis, float angle );
	void setRotation( glm::mat3 rot );
	glm::mat4 getRotationMatrix();

	glm::mat4 getLinearTransformation();
	glm::mat4 getTransformation();
};

