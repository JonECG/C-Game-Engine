#pragma once


#include "GeneralGLWindow.h"

struct DebugShape
{
	GeneralGlWindow::Renderable *renderable;
	GeneralGlWindow::ShaderInfo *shader;
	float timeLeft;
	glm::mat4 whereMatrix;
	bool useDepth;
	glm::vec4 color;
	bool visible;

	void draw( GeneralGlWindow *window );
};

#ifdef DEBUG_SHAPES
#include "ShapeGenerator.h"
#include "Engine.h"
#include "glm/gtc/matrix_transform.hpp"
//#include <GL/glew.h>


class DebugShapes
{
	static ENGINE_SHARED GeneralGlWindow::Renderable *cube, *sphere, *crossHair, *line, *vector;

	static ENGINE_SHARED int numberOfDebugs;

	static ENGINE_SHARED GeneralGlWindow *window;

	static ENGINE_SHARED DebugShape *shapes;

	
	static ENGINE_SHARED DebugShape* addShape( GeneralGlWindow::Renderable *geo, GeneralGlWindow::ShaderInfo *shad, glm::mat4 transform, glm::vec4 color, float lifeTime, bool useDepth );

	inline static void cullColor( Neumont::ShapeData *shape );

public:
	static ENGINE_SHARED void init( GeneralGlWindow *window );

	inline static void update( float dt );

	inline static void addCube( GeneralGlWindow::ShaderInfo *shad, glm::mat4 transform, glm::vec4 color = glm::vec4(1,1,1,1), float lifeTime = -1, bool useDepth = true);

	inline static void addSphere( GeneralGlWindow::ShaderInfo *shad, glm::mat4 transform, glm::vec4 color = glm::vec4(1,1,1,1), float lifeTime = -1, bool useDepth = true);

	inline static void addPoint( GeneralGlWindow::ShaderInfo *shad, glm::mat4 transform, float lifeTime = -1, bool useDepth = true);

	inline static void addLine( GeneralGlWindow::ShaderInfo *shad, glm::vec3 from, glm::vec3 to, glm::vec4 color = glm::vec4(1,1,1,1), float lifeTime = -1, bool useDepth = true);

	inline static void addVector( GeneralGlWindow::ShaderInfo *shad, glm::vec3 tailPosition, glm::vec3 vec, glm::vec4 color = glm::vec4(1,1,1,1), float lifeTime = -1, bool useDepth = true);

	static ENGINE_SHARED void draw();
};

#include "DebugShapes.inl"

#endif

#ifndef DEBUG_SHAPES

class DebugShapes
{

public:
	inline static void init( GeneralGlWindow *window ){}

	inline static void update( float dt ){}

	inline static void addCube( GeneralGlWindow::ShaderInfo *shad, glm::mat4 transform, glm::vec4 color = glm::vec4(1,1,1,1), float lifeTime = -1, bool useDepth = true){}

	inline static void addSphere( GeneralGlWindow::ShaderInfo *shad, glm::mat4 transform, glm::vec4 color = glm::vec4(1,1,1,1), float lifeTime = -1, bool useDepth = true){}

	inline static void addPoint( GeneralGlWindow::ShaderInfo *shad, glm::mat4 transform, float lifeTime = -1, bool useDepth = true){}

	inline static void addLine( GeneralGlWindow::ShaderInfo *shad, glm::vec3 from, glm::vec3 to, glm::vec4 color = glm::vec4(1,1,1,1), float lifeTime = -1, bool useDepth = true){}

	inline static void addVector( GeneralGlWindow::ShaderInfo *shad, glm::vec3 tailPosition, glm::vec3 vec, glm::vec4 color = glm::vec4(1,1,1,1), float lifeTime = -1, bool useDepth = true){}

	inline static void draw(){}
};

#endif