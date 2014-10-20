#include "Component.h"
#include "Entity.h"
#include <typeinfo>
#include <iostream>
#include <string>

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
	id = 0;
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

void Component::draw()
{
}

void Component::update( float dt )
{
}
