#include "UniformInfo.h"
#include <GL\glew.h>


void UniformInfo::send() const
{
	if (location != -1)
	{
		switch( type )
		{
		case ParameterType::PT_INT:
			glUniform1i( location, *((int*)data) );
			break;
		case ParameterType::PT_FLOAT:
			glUniform1f( location, *((float*)data) );
			break;

		case ParameterType::PT_VEC2:
			glUniform2fv( location, 1, (float*)data );
			break;
		case ParameterType::PT_VEC3:
			glUniform3fv( location, 1, (float*)data );
			break;
		case ParameterType::PT_VEC4:
			glUniform4fv( location, 1, (float*)data );
			break;

		case ParameterType::PT_MAT3:
			glUniformMatrix3fv( location, 1, false, (float*)data );
			break;
		case ParameterType::PT_MAT4:
			glUniformMatrix4fv( location, 1, false, (float*)data );
			break;
		}
	}
}
