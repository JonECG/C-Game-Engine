#pragma once
#include "definition.h"
#include "JankHashMap.h"
#include <typeinfo>
//namespace std
//{ 
//	template< class K, class V > 
//	class unordered_map;
//};

class Component;
class EXPORT Entity
{
	JankHashMap< int, Component* > * components;

	friend class Stage;
	Stage * stage;
public:
	Entity();
	~Entity();

	Stage * getStage();

	void addComponent( Component* comp );
	void clearComponents();

	inline int hsh( const char * );

	template<class ComponentType>
		inline ComponentType* gc();

	template<class ComponentType>
		inline ComponentType* getComponent();

	template<class ComponentType>
		inline ComponentType* removeComponent();

	template<class ComponentType>
		inline void deleteComponent();
};




//Too lazy for .inl
#include <string>
#include <iostream>
int Entity::hsh( const char * s )
{
	//std::cout << "Hashing: \"" << s << "\" with " << s[6] << " and " << s[12] << std::endl;
	return s[6] + 30*s[12];
}

template<class ComponentType>
ComponentType* Entity::gc()
{
	return getComponent<ComponentType>();
}



template<typename ComponentType>
ComponentType* Entity::getComponent()
{
	auto pos = hsh(typeid(ComponentType).name());

	if(components->has( pos ))
    {
		return reinterpret_cast<ComponentType*>((*components).get( pos ));
    }
    else 
    {
        return nullptr;
    }
}
template<typename ComponentType>
ComponentType* Entity::removeComponent()
{
	ComponentType* result = nullptr;
	//auto pos = &typeid(ComponentType);
	auto pos = hsh(typeid(ComponentType).name());

	if(components->has( pos ))
    {
		//result = reinterpret_cast<ComponentType*>((*components)[pos]);
		//components->erase( pos );
		result = reinterpret_cast<ComponentType*>((*components).remove( pos ));
		result->parent = nullptr;
	}
	else
	{
		throw "This type of component does not exist";
	}

	return result;
}
template<typename ComponentType>
void Entity::deleteComponent()
{
	delete removeComponent<ComponentType>();
}