#pragma once
#include "definition.h"
struct Geometry;
class ContentManager;
namespace ShapeGenerator
{
	extern EXPORT Geometry * createCube( ContentManager * content );
	extern EXPORT Geometry * createSphere( ContentManager * content, int subdivisions );
};

