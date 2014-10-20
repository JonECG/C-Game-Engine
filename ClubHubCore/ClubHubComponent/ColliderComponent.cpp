#include "ColliderComponent.h"
#include "TransformComponent.h"
#include "Entity.h"
enum Shapes{ CUBE, SPHERE };

const float EPSILON = 0.001f;
ColliderComponent::ColliderComponent()
{
}

bool ColliderComponent::collidesWith( ColliderComponent * other, glm::vec3 * collisionNormal, float * interpenetration )
{
	TransformComponent *thisTrans = parent->getComponent<TransformComponent>(), *otherTrans = other->parent->getComponent<TransformComponent>();


	if( this->type == Shapes::SPHERE && other->type == Shapes::SPHERE )
	{
		glm::vec3 diff = thisTrans->getTranslation() - otherTrans->getTranslation();
		float distSqr = glm::dot( diff, diff );
		if( distSqr < (this->radius+other->radius)*(this->radius+other->radius) )
		{
			(*interpenetration) = ( this->radius + other->radius ) - std::sqrt(distSqr) + EPSILON;
			(*collisionNormal) = glm::normalize( diff );
			return true;
		}
	}

	if( this->type == Shapes::CUBE && other->type == Shapes::CUBE )
	{
		float amnt;
		float mostInter;
		float closestRat;
		glm::vec3 norm;

		//check the X axis
		amnt = std::abs( thisTrans->getTranslation().x - otherTrans->getTranslation().x ) / (this->bounds.x + other->bounds.x);
		if( amnt < 1 )
		{
			closestRat = amnt;
			mostInter = (this->bounds.x + other->bounds.x) - closestRat * (this->bounds.x + other->bounds.x);
			norm = glm::vec3( ( thisTrans->getTranslation().x > otherTrans->getTranslation().x ) ? 1 : -1, 0, 0 );
			//check the Y axis
			amnt = std::abs( thisTrans->getTranslation().y - otherTrans->getTranslation().y ) / (this->bounds.y + other->bounds.y);
			if( amnt < 1 )
			{
				if( amnt > closestRat )
				{
					closestRat = amnt;
					mostInter = (this->bounds.y + other->bounds.y) - closestRat * (this->bounds.y + other->bounds.y);
					norm = glm::vec3( 0, ( thisTrans->getTranslation().y > otherTrans->getTranslation().y ) ? 1 : -1, 0 );
				}
				//check the Z axis
				amnt = std::abs( thisTrans->getTranslation().z - otherTrans->getTranslation().z ) / (this->bounds.z + other->bounds.z);
				if( amnt < 1 )
				{
					if( amnt > closestRat )
					{
						closestRat = amnt;
						mostInter = (this->bounds.z + other->bounds.z) - closestRat * (this->bounds.z + other->bounds.z);
						norm = glm::vec3( 0, 0, ( thisTrans->getTranslation().z > otherTrans->getTranslation().z ) ? 1 : -1 );
					}

					(*interpenetration) = mostInter + EPSILON;
					(*collisionNormal) = norm;
					return true;
				}
			}
		}
	}

	return false;
}

void ColliderComponent::setAsBox( glm::vec3 bounds )
{
	type = Shapes::CUBE;
	this->bounds = bounds;
}

void ColliderComponent::setAsSphere( float radius )
{
	type = Shapes::SPHERE;
	this->radius = radius;
}