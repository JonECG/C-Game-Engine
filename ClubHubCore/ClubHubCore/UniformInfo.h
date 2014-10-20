#pragma once
#include "definition.h"
#include "ParameterType.h"

struct EXPORT UniformInfo
{
	int location;

	const void* data;
	const char* name;

	ParameterType type;

	inline UniformInfo(){};
	inline UniformInfo( int locationIn, const void* dataIn, ParameterType typeIn );
	inline UniformInfo( const char * name, const void* dataIn, ParameterType typeIn );

	void send() const;

};

UniformInfo::UniformInfo( int locationIn, const void* dataIn, ParameterType typeIn )
{
	this->location = locationIn;
	data = dataIn;
	type = typeIn;
}

UniformInfo::UniformInfo( const char * name, const void* dataIn, ParameterType typeIn )
{
	this->location = -1;
	this->name = name;
	data = dataIn;
	type = typeIn;
}