#include "Vertex.h"
#include "Geometry.h"
#include "ParameterType.h"
#include "glm\glm.hpp"

unsigned int Vertex::POSITION_OFFSET = 0;
unsigned int Vertex::COLOR_OFFSET = Vertex::POSITION_OFFSET + sizeof( glm::vec3 );
unsigned int Vertex::NORMAL_OFFSET = Vertex::COLOR_OFFSET + sizeof( glm::vec4 );
unsigned int Vertex::UV_OFFSET = Vertex::NORMAL_OFFSET + sizeof( glm::vec3 );
unsigned int Vertex::STRIDE = sizeof( Vertex );

void Vertex::setAttributes( Geometry* geo )
{
	//Position
	geo->addAttribute( 0, ParameterType::PT_VEC3, POSITION_OFFSET, STRIDE );
	//Color
	geo->addAttribute( 1, ParameterType::PT_VEC4, COLOR_OFFSET, STRIDE );
	//Normal
	geo->addAttribute( 2, ParameterType::PT_VEC3, NORMAL_OFFSET, STRIDE );
	//UV
	geo->addAttribute( 3, ParameterType::PT_VEC2, UV_OFFSET, STRIDE );
}

glm::vec3 Vertex::getPosition()
{
	return *reinterpret_cast<glm::vec3*>( &data[POSITION_OFFSET/sizeof(float)] );
}
glm::vec4 Vertex::getColor()
{
	return *reinterpret_cast<glm::vec4*>( &data[COLOR_OFFSET/sizeof(float)] );
}
glm::vec3 Vertex::getNormal()
{
	return *reinterpret_cast<glm::vec3*>( &data[NORMAL_OFFSET/sizeof(float)] );
}
glm::vec2 Vertex::getUv()
{
	return *reinterpret_cast<glm::vec2*>( &data[UV_OFFSET/sizeof(float)] );
}

void Vertex::setPosition( glm::vec3 position )
{
	*reinterpret_cast<glm::vec3*>( &data[POSITION_OFFSET/sizeof(float)] ) = position;
}
void Vertex::setColor( glm::vec4 color )
{
	*reinterpret_cast<glm::vec4*>( &data[COLOR_OFFSET/sizeof(float)] ) = color;
}
void Vertex::setNormal( glm::vec3 normal )
{
	*reinterpret_cast<glm::vec3*>( &data[NORMAL_OFFSET/sizeof(float)] ) = normal;
}
void Vertex::setUv( glm::vec2 uv )
{
	*reinterpret_cast<glm::vec2*>( &data[UV_OFFSET/sizeof(float)] ) = uv;
}