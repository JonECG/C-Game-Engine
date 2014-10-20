#include "Entity.h"
#include "Component.h"

Entity::Entity()
{
	currentComponent = 0;
}
Entity::~Entity()
{
	clearComponents();
}

int Entity::findComponent( int id )
{
	for( int i = 0; i < currentComponent; i++ )
	{
		if( components[i]->getId() == id )
			return i;
	}
	return -1;
}

void Entity::pullComponent( int index )
{
	currentComponent--;
	components[index] = components[currentComponent];
}

void Entity::addComponent( Component* comp )
{
	if( comp->parent != nullptr )
		throw "This component already has a parent, you dumb bimbo!";

	components[currentComponent++] = comp;
	comp->parent = this;
}

void Entity::clearComponents()
{
	for( int i = 0; i < currentComponent; i++ )
	{
		delete components[i];
	}
	currentComponent = 0;
}

void Entity::update( float dt )
{
	for( int i = 0; i < currentComponent; i++ )
	{
		components[i]->update( dt );
	}
}

void Entity::draw()
{
	for( int i = 0; i < currentComponent; i++ )
	{
		components[i]->draw();
	}
}