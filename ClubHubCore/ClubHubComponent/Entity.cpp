#include "Entity.h"
#include "Component.h"
#include <typeinfo>
static const int MAX_COMPONENTS = 10;

Entity::Entity()
{
	components = new JankHashMap< int, Component * >;//( MAX_COMPONENTS );
}
Entity::~Entity()
{
	clearComponents();
	delete components;
}

Stage * Entity::getStage()
{
	return stage;
}

void Entity::addComponent( Component* comp )
{
	//auto pos = &typeid(*comp);
	auto pos = hsh(typeid(*comp).name());

	if(components->has(pos))
    {
		throw "This type of component already exists in the entity";
	}
	else
	{
		(*components).put( pos, comp );
	}

	comp->parent = this;
}

void Entity::clearComponents()
{
	/*for( auto i = components->begin(); i != components->end(); i++ )
	{
		delete i->second;
	}*/

	for( int i = 0; i < components->count(); i++ )
	{
		delete components->getInserted( i );
	}

	components->clear();
}