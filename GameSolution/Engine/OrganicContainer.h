#ifndef ORGANICCONTAINER_H
#define ORGANICCONTAINER_H

#include "ExportHeader.h"
#include "Core.h"
#include "IOrganic.h"
#include "Matrix4.h"


template <class O>
class OrganicContainer
{
protected:
	O ** current;
	int size;
	int head;

public:
	inline OrganicContainer(){};
	inline ~OrganicContainer();
	inline void draw( Core::Graphics& g ) const;
	inline void draw( Core::Graphics& g, Matrix4& globalTransform ) const;
	inline void update( float dt );
	inline void reset();
	inline O** getContained();
	inline int getSize();
};

#include "OrganicContainer.inl"

#endif