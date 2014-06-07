#include "DebugShapes.h"

#ifdef DEBUG_SHAPES

GeneralGlWindow::Renderable *DebugShapes::cube = nullptr;
GeneralGlWindow::Renderable *DebugShapes::sphere = nullptr;
GeneralGlWindow::Renderable *DebugShapes::crossHair = nullptr;
GeneralGlWindow::Renderable *DebugShapes::line = nullptr;
GeneralGlWindow::Renderable *DebugShapes::vector = nullptr;

int DebugShapes::numberOfDebugs = 0;

GeneralGlWindow *DebugShapes::window = nullptr;

DebugShape* DebugShapes::shapes = nullptr;

void DebugShape::draw( GeneralGlWindow *window )
{
	renderable->where = whereMatrix;
	renderable->howShader = shader;
	window->setUniformParameter( renderable, "colorInfluence", GeneralGlWindow::ParameterType::PT_VEC4, (float*)(&color) );
	renderable->draw();
}

DebugShape* DebugShapes::addShape( GeneralGlWindow::Renderable *rend, GeneralGlWindow::ShaderInfo *shad, glm::mat4 transform, glm::vec4 color, float lifeTime, bool useDepth )
{
	bool found = false;
	DebugShape* current = &shapes[numberOfDebugs];
	for( int i = 0; i < numberOfDebugs; i++ )
	{
		if (!found && shapes[i].timeLeft <= 0 && shapes[i].timeLeft != -1 )
		{
			current = &shapes[i];
			found = true;
		}
	}
	if( !found )
	{
		numberOfDebugs++;
		std::cout << numberOfDebugs << std::endl;
	}

	//std::cout << "Made a debug shape current number: " << numberOfDebugs << std::endl; 
	current->renderable = rend;
	current->shader = shad;
	current->color = color;
	current->whereMatrix = transform;
	current->timeLeft = lifeTime;
	current->visible = true;
	current->useDepth = useDepth;

	//window->addUniformParameter( current->renderable, "colorInfluence", GeneralGlWindow::ParameterType::PT_VEC4, (float*)(&current->color) );

	return current;
}

void DebugShapes::init( GeneralGlWindow *window )
{
	DebugShapes::window = window;
	shapes = (DebugShape*)malloc( sizeof( DebugShape) * 2000 );

	Neumont::ShapeData cubeData = Neumont::ShapeGenerator::makeCube();
	cullColor( &cubeData );
	GeneralGlWindow::GeometryInfo* geocube = window->addGeometry( cubeData.verts, cubeData.numVerts, cubeData.indices, cubeData.numIndices, GL_TRIANGLES );
	window->setUpAttribs( geocube );
	cube = window->addRenderable( geocube, glm::mat4(), nullptr, nullptr );

	Neumont::ShapeData sphereData = Neumont::ShapeGenerator::makeSphere( 24 );
	cullColor( &sphereData );
	GeneralGlWindow::GeometryInfo* geosphere = window->addGeometry( sphereData.verts, sphereData.numVerts, sphereData.indices, sphereData.numIndices, GL_TRIANGLES );
	window->setUpAttribs( geosphere );
	sphere = window->addRenderable( geosphere, glm::mat4(), nullptr, nullptr );

	Neumont::Vertex pointVData[6];
	//X
	pointVData[0].position = glm::vec3( -1, 0, 0 );	pointVData[0].color = glm::vec4( 0, 0, 0, 1 );
	pointVData[1].position = glm::vec3( 1, 0, 0 );	pointVData[1].color = glm::vec4( 1, 0, 0, 1 );

	//Y
	pointVData[2].position = glm::vec3( 0, -1, 0 );	pointVData[2].color = glm::vec4( 0, 0, 0, 1 );
	pointVData[3].position = glm::vec3( 0, 1, 0 );	pointVData[3].color = glm::vec4( 0, 1, 0, 1 );

	//Z
	pointVData[4].position = glm::vec3( 0, 0, -1 );	pointVData[4].color = glm::vec4( 0, 0, 0, 1 );
	pointVData[5].position = glm::vec3( 0, 0, 1 );	pointVData[5].color = glm::vec4( 0, 0, 1, 1 );
	for( int i = 0; i < 6; i++ )
	{
		pointVData[i].normal = glm::vec3( 0, 0, 0 );
		pointVData[i].uv = glm::vec2( 0, 0 );
	}
	ushort pointIData[] = { 0, 1, 2, 3, 4, 5 }; 
	GeneralGlWindow::GeometryInfo* geocrossHair = window->addGeometry( pointVData, 6, pointIData, 6, GL_LINES );
	window->setUpAttribs( geocrossHair );
	crossHair = window->addRenderable( geocrossHair, glm::mat4(), nullptr, nullptr );

	//X
	pointVData[0].position = glm::vec3( 0, 0, 0 );	pointVData[0].color = glm::vec4( 0, 0, 0, 1 );
	pointVData[1].position = glm::vec3( 1, 0, 0 );	pointVData[1].color = glm::vec4( 1, 1, 1, 1 );

	GeneralGlWindow::GeometryInfo* geovector = window->addGeometry( pointVData, 2, pointIData, 2, GL_LINES );
	window->setUpAttribs( geovector );
	vector = window->addRenderable( geovector, glm::mat4(), nullptr, nullptr );

	pointVData[0].color = glm::vec4( 1, 1, 1, 1 );
	GeneralGlWindow::GeometryInfo* geoline = window->addGeometry( pointVData, 2, pointIData, 2, GL_LINES );
	window->setUpAttribs( geoline );
	line = window->addRenderable( geoline, glm::mat4(), nullptr, nullptr );
}

void DebugShapes::draw()
{
	for( int i = 0; i < numberOfDebugs; i++ )
	{
		if ( shapes[i].visible && shapes[i].useDepth )
			shapes[i].draw( window );
	}

	glClear(GL_DEPTH_BUFFER_BIT);

	for( int i = 0; i < numberOfDebugs; i++ )
	{
		if ( shapes[i].visible && !shapes[i].useDepth )
			shapes[i].draw( window );
	}
}

#endif