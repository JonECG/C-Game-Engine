#include "ShapeGenerator.h"
#include "Geometry.h"
#include "ContentManager.h"
#include "Vertex.h"
#include "GL\glew.h"


Geometry * ShapeGenerator::createCube( ContentManager * cont )
{
	//cont->addGeometry( 
	//Neumont::ShapeData result = Neumont::ShapeGenerator::makeCube();

	Vertex * v = new Vertex[ 24 ];
	int i = 0;

	v[i].setPosition( glm::vec3(-1.0f, +1.0f, +1.0f) );        // 0
	v[i].setColor( glm::vec4(+0.0f, +1.0f, +0.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(+0.0f, +1.0f, +0.0f) );        // Normal
	v[i].setUv( glm::vec2(+0.0f, +1.0f) );               // UV
	i++;

	v[i].setPosition(  glm::vec3(+1.0f, +1.0f, +1.0f) );        // 1
	v[i].setColor( glm::vec4(+0.0f, +1.0f, +0.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(+0.0f, +1.0f, +0.0f) );        // Normal
	v[i].setUv( glm::vec2(+1.0f, +1.0f) );               // UV
	i++;

	v[i].setPosition(  glm::vec3(+1.0f, +1.0f, -1.0f) );        // 2
	v[i].setColor( glm::vec4(+0.0f, +1.0f, +0.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(+0.0f, +1.0f, +0.0f) );        // Normal
	v[i].setUv( glm::vec2(+1.0f, +0.0f) );               // UV
	i++;

	v[i].setPosition( glm::vec3(-1.0f, +1.0f, -1.0f) );        // 3
	v[i].setColor( glm::vec4(+0.0f, +1.0f, +0.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(+0.0f, +1.0f, +0.0f) );        // Normal
	v[i].setUv( glm::vec2(+0.0f, +0.0f) );               // UV
	i++;

	v[i].setPosition( glm::vec3(-1.0f, +1.0f, -1.0f) );        // 4
	v[i].setColor( glm::vec4(+1.0f, +1.0f, +0.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(+0.0f, +0.0f, -1.0f) );        // Normal
	v[i].setUv( glm::vec2(+0.0f, +1.0f) );               // UV
	i++;

	v[i].setPosition( glm::vec3(+1.0f, +1.0f, -1.0f) );        // 5
	v[i].setColor( glm::vec4(+1.0f, +1.0f, +0.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(+0.0f, +0.0f, -1.0f) );        // Normal
	v[i].setUv( glm::vec2(+1.0f, +1.0f) );               // UV
	i++;

	v[i].setPosition( glm::vec3(+1.0f, -1.0f, -1.0f) );        // 6
	v[i].setColor( glm::vec4(+1.0f, +1.0f, +0.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(+0.0f, +0.0f, -1.0f) );        // Normal
	v[i].setUv( glm::vec2(+1.0f, +0.0f) );               // UV
	i++;

	v[i].setPosition( glm::vec3(-1.0f, -1.0f, -1.0f) );        // 7
	v[i].setColor( glm::vec4(+1.0f, +1.0f, +0.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(+0.0f, +0.0f, -1.0f) );        // Normal
	v[i].setUv( glm::vec2(+0.0f, +0.0f) );               // UV
	i++;

	v[i].setPosition( glm::vec3(+1.0f, +1.0f, -1.0f) );        // 8
	v[i].setColor( glm::vec4(+1.0f, +0.0f, +0.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(+1.0f, +0.0f, +0.0f) );        // Normal
	v[i].setUv( glm::vec2(+0.0f, +1.0f) );               // UV
	i++;

	v[i].setPosition( glm::vec3(+1.0f, +1.0f, +1.0f) );        // 9
	v[i].setColor( glm::vec4(+1.0f, +0.0f, +0.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(+1.0f, +0.0f, +0.0f) );        // Normal
	v[i].setUv( glm::vec2(+1.0f, +1.0f) );               // UV
	i++;

	v[i].setPosition( glm::vec3(+1.0f, -1.0f, +1.0f) );        // 10
	v[i].setColor( glm::vec4(+1.0f, +0.0f, +0.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(+1.0f, +0.0f, +0.0f) );        // Normal
	v[i].setUv( glm::vec2(+1.0f, +0.0f) );               // UV
	i++;

	v[i].setPosition( glm::vec3(+1.0f, -1.0f, -1.0f) );        // 11
	v[i].setColor( glm::vec4(+1.0f, +0.0f, +0.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(+1.0f, +0.0f, +0.0f) );        // Normal
	v[i].setUv( glm::vec2(+0.0f, +0.0f) );               // UV
	i++;

	v[i].setPosition( glm::vec3(-1.0f, +1.0f, +1.0f) );        // 12
	v[i].setColor( glm::vec4(+0.0f, +1.0f, +1.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(-1.0f, +0.0f, +0.0f) );        // Normal
	v[i].setUv( glm::vec2(+0.0f, +1.0f) );               // UV
	i++;

	v[i].setPosition( glm::vec3(-1.0f, +1.0f, -1.0f) );        // 13
	v[i].setColor( glm::vec4(+0.0f, +1.0f, +1.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(-1.0f, +0.0f, +0.0f) );        // Normal
	v[i].setUv( glm::vec2(+1.0f, +1.0f) );               // UV
	i++;

	v[i].setPosition( glm::vec3(-1.0f, -1.0f, -1.0f) );        // 14
	v[i].setColor( glm::vec4(+0.0f, +1.0f, +1.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(-1.0f, +0.0f, +0.0f) );        // Normal
	v[i].setUv( glm::vec2(+1.0f, +0.0f) );               // UV
	i++;

	v[i].setPosition( glm::vec3(-1.0f, -1.0f, +1.0f) );        // 15
	v[i].setColor( glm::vec4(+0.0f, +1.0f, +1.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(-1.0f, +0.0f, +0.0f) );        // Normal
	v[i].setUv( glm::vec2(+0.0f, +0.0f) );               // UV
	i++;

	v[i].setPosition( glm::vec3(+1.0f, +1.0f, +1.0f) );        // 16
	v[i].setColor( glm::vec4(+0.0f, +0.0f, +1.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(+0.0f, +0.0f, +1.0f) );        // Normal
	v[i].setUv( glm::vec2(+0.0f, +1.0f) );               // UV
	i++;

	v[i].setPosition( glm::vec3(-1.0f, +1.0f, +1.0f) );        // 17
	v[i].setColor( glm::vec4(+0.0f, +0.0f, +1.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(+0.0f, +0.0f, +1.0f) );        // Normal
	v[i].setUv( glm::vec2(+1.0f, +1.0f) );               // UV
	i++;

	v[i].setPosition( glm::vec3(-1.0f, -1.0f, +1.0f) );        // 18
	v[i].setColor( glm::vec4(+0.0f, +0.0f, +1.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(+0.0f, +0.0f, +1.0f) );        // Normal
	v[i].setUv( glm::vec2(+1.0f, +0.0f) );               // UV
	i++;

	v[i].setPosition( glm::vec3(+1.0f, -1.0f, +1.0f) );        // 19
	v[i].setColor( glm::vec4(+0.0f, +0.0f, +1.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(+0.0f, +0.0f, +1.0f) );        // Normal
	v[i].setUv( glm::vec2(+0.0f, +0.0f) );               // UV
	i++;

	v[i].setPosition( glm::vec3(+1.0f, -1.0f, -1.0f) );        // 20
	v[i].setColor( glm::vec4(+1.0f, +0.3f, +1.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(+0.0f, -1.0f, +0.0f) );        // Normal
	v[i].setUv( glm::vec2(+0.0f, +1.0f) );               // UV
	i++;

	v[i].setPosition( glm::vec3(-1.0f, -1.0f, -1.0f) );        // 21
	v[i].setColor( glm::vec4(+1.0f, +0.3f, +1.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(+0.0f, -1.0f, +0.0f) );        // Normal
	v[i].setUv( glm::vec2(+1.0f, +1.0f) );               // UV
	i++;

	v[i].setPosition( glm::vec3(-1.0f, -1.0f, +1.0f) );        // 22
	v[i].setColor( glm::vec4(+1.0f, +0.3f, +1.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(+0.0f, -1.0f, +0.0f) );        // Normal
	v[i].setUv( glm::vec2(+1.0f, +0.0f) );               // UV
	i++;

	v[i].setPosition( glm::vec3(+1.0f, -1.0f, +1.0f) );        // 23
	v[i].setColor( glm::vec4(+1.0f, +0.3f, +1.0f, +1.0f) ); // Color
	v[i].setNormal( glm::vec3(+0.0f, -1.0f, +0.0f) );        // Normal
	v[i].setUv( glm::vec2(+0.0f, +0.0f) );               // UV
 
	unsigned short stackIndices[] = {
		0,   1,  2,  0,  2,  3, // Top
		4,   5,  6,  4,  6,  7, // Front
		8,   9, 10,  8, 10, 11, // Right 
		12, 13, 14, 12, 14, 15, // Left
		16, 17, 18, 16, 18, 19, // Back
		20, 22, 21, 20, 23, 22, // Bottom
	};	

	/*
	//Normal: <0,1,0> -- Tangent: <1,0,0,-1>
	result.verts[i++].setColor( glm::vec4( 1, 0, 0, -1 );
	//Normal: <0,1,0> -- Tangent: <1,0,0,-1>
	result.verts[i++].setColor( glm::vec4( 1, 0, 0, -1 );
	//Normal: <0,1,0> -- Tangent: <1,0,0,-1>
	result.verts[i++].setColor( glm::vec4( 1, 0, 0, -1 );
	//Normal: <0,1,0> -- Tangent: <1,0,0,-1>
	result.verts[i++].setColor( glm::vec4( 1, 0, 0, -1 );

	//Normal: <0,0,-1> -- Tangent: <1,0,0,-1>
	result.verts[i++].setColor( glm::vec4( 1, 0, 0, -1 );
	//Normal: <0,0,-1> -- Tangent: <1,0,0,-1>
	result.verts[i++].setColor( glm::vec4( 1, 0, 0, -1 );
	//Normal: <0,0,-1> -- Tangent: <1,0,0,-1>
	result.verts[i++].setColor( glm::vec4( 1, 0, 0, -1 );
	//Normal: <0,0,-1> -- Tangent: <1,0,0,-1>
	result.verts[i++].setColor( glm::vec4( 1, 0, 0, -1 );

	//Normal: <1,0,0> -- Tangent: <0,0,1,-1>
	result.verts[i++].setColor( glm::vec4( 0, 0, 1, -1 );
	//Normal: <1,0,0> -- Tangent: <0,0,1,-1>
	result.verts[i++].setColor( glm::vec4( 0, 0, 1, -1 );
	//Normal: <1,0,0> -- Tangent: <0,0,1,-1>
	result.verts[i++].setColor( glm::vec4( 0, 0, 1, -1 );
	//Normal: <1,0,0> -- Tangent: <0,0,1,-1>
	result.verts[i++].setColor( glm::vec4( 0, 0, 1, -1 );

	//Normal: <-1,0,0> -- Tangent: <0,0,-1,-1>
	result.verts[i++].setColor( glm::vec4( 0, 0, -1, -1 );
	//Normal: <-1,0,0> -- Tangent: <0,0,-1,-1>
	result.verts[i++].setColor( glm::vec4( 0, 0, -1, -1 );
	//Normal: <-1,0,0> -- Tangent: <0,0,-1,-1>
	result.verts[i++].setColor( glm::vec4( 0, 0, -1, -1 );
	//Normal: <-1,0,0> -- Tangent: <0,0,-1,-1>
	result.verts[i++].setColor( glm::vec4( 0, 0, -1, -1 );

	//Normal: <0,0,1> -- Tangent: <-1,0,0,-1>
	result.verts[i++].setColor( glm::vec4( -1, 0, 0, -1 );
	//Normal: <0,0,1> -- Tangent: <-1,0,0,-1>
	result.verts[i++].setColor( glm::vec4( -1, 0, 0, -1 );
	//Normal: <0,0,1> -- Tangent: <-1,0,0,-1>
	result.verts[i++].setColor( glm::vec4( -1, 0, 0, -1 );
	//Normal: <0,0,1> -- Tangent: <-1,0,0,-1>
	result.verts[i++].setColor( glm::vec4( -1, 0, 0, -1 );

	//Normal: <0,-1,0> -- Tangent: <-1,0,0,1>
	result.verts[i++].setColor( glm::vec4( -1, 0, 0, 1 );
	//Normal: <0,-1,0> -- Tangent: <-1,0,0,1>
	result.verts[i++].setColor( glm::vec4( -1, 0, 0, 1 );
	//Normal: <0,-1,0> -- Tangent: <-1,0,0,1>
	result.verts[i++].setColor( glm::vec4( -1, 0, 0, 1 );
	//Normal: <0,-1,0> -- Tangent: <-1,0,0,1>
	result.verts[i++].setColor( glm::vec4( -1, 0, 0, 1 );
	*/
	Geometry * ret = cont->addGeometry( v, 24, stackIndices, 36, GL_TRIANGLES );
	Vertex::setAttributes( ret );

	delete [] v;

	return ret;
}