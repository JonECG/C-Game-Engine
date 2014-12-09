#include "Component.h"
#include "Entity.h"
#include <typeinfo>
#include <iostream>
#include <string>
#include "glm\glm.hpp"

//int hash( const char * string )
//{
//    int result = 0;
//	int index = 0;
//    while( string[index] != '\0' )
//	{
//        result += string[index] * pow(31, i);
//    }
//}

Component::Component()
{
	this->parent = nullptr;
	inited = false;
	id = 0;
}

Component::~Component()
{
}

Entity * Component::getParent()
{
	return this->parent;
}

int Component::getId()
{
	if( id == 0 )
	{
		//hashes the name of the class
		id = std::hash<std::string>()(std::string( typeid( *this ).name() ));
		std::cout << "Set ID of component " << typeid( *this ).name() << " with id " << this->id << std::endl;
	}
	return id;
}

void Component::init()
{
}

void Component::destroy()
{
}

void Component::draw()
{
}

void Component::onCollide( Entity * other, glm::vec3 collisionNormal, float interpenetration )
{
	//std::cout << "COLLIDED with " << interpenetration << std::endl;
	other;
	collisionNormal;
	interpenetration;
}

void Component::update( float dt )
{
	dt;
}

void Component::earlyUpdate( float dt )
{
	dt;
}

void Component::lateUpdate( float dt )
{
	dt;
}
