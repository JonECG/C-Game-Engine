#pragma once
#include "Engine.h"

class Component;
class ENGINE_SHARED Entity
{
	static const int MAX_COMPONENTS = 10;
	Component* components[ MAX_COMPONENTS ];
	int currentComponent;
	int findComponent( int id );
	void pullComponent( int index );

public:
	Entity();
	~Entity();


	void addComponent( Component* comp );
	void clearComponents();

	template<class ComponentType>
		inline ComponentType* gc();

	template<class ComponentType>
		inline ComponentType* getComponent();

	template<class ComponentType>
		inline ComponentType* removeComponent();

	template<class ComponentType>
		inline void deleteComponent();
	

	void update( float dt );
	void draw();
};




//Too lazy for .inl
#include <typeinfo>
#include <string>

template<class ComponentType>
ComponentType* Entity::gc()
{
	return getComponent<ComponentType>();
}

template<class ComponentType>
ComponentType* Entity::getComponent()
{
	int id = std::hash<std::string>()(std::string(typeid( ComponentType ).name()));
	//std::cout << "    Searching for component " << typeid( ComponentType ).name() << " with id " << id << std::endl;
	int ind = findComponent(id);
	return (ind != -1) ? reinterpret_cast<ComponentType*>(components[ind]) : nullptr;
}
template<class ComponentType>
ComponentType* Entity::removeComponent()
{
	int id = std::hash<const char *>()(typeid( ComponentType ).name());
	int ind = findComponent(id);
	ComponentType* result = nullptr;
	if( ind == -1 )
	{
		throw "This type of component does not exist";
	}
	else
	{
		result = components[ind];
		result->parent = nullptr;
		pullComponent( ind );
	}
	return result;
}
template<class ComponentType>
void Entity::deleteComponent()
{
	int id = std::hash<const char *>()(typeid( ComponentType ).name());
	int ind = findComponent(id);
	if( ind == -1 )
	{
		throw "This type of component does not exist";
	}
	else
	{
		delete components[ind];
		pullComponent( ind );
	}
}