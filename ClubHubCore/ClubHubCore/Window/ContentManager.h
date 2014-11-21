#pragma once

#include "definition.h"
#include "Renderer\Buffer.h"
#include "Renderer\Geometry.h"

struct Shader;
struct Texture;
struct Vertex;
struct Font;

#include <map>
/*
namespace std
{
	template<class _Ty>
	struct less;

	template<class _Ty>
	class allocator;

	template<class _Ty1,
	class _Ty2>
	struct pair;

	template<class _Kty,
	class _Ty,
	class _Pr = less<_Kty>,
	class _Alloc = allocator<pair<const _Kty, _Ty> > >

	class map;

	template<class _Elem,
	class _Traits,
	class _Alloc>
	class basic_string;
	typedef basic_string<char, char_traits<char>, allocator<char> > string;
}*/

class EXPORT ContentManager
{
	int currentGeometry;
	static const int NUMBER_OF_BUFFERS = 10;
	Geometry geos[100];
	Buffer buffers[NUMBER_OF_BUFFERS];
	std::map<std::string, Shader*> shaders;
	std::map<std::string, Texture*> textures;
	std::map<std::string, Font*> fonts;

	Buffer* nextFreeBuffer( unsigned int size );
	Geometry* geometryFromFile( const char* path );
public:
	ContentManager();
	~ContentManager();

	Geometry* addGeometry( const Vertex* verts, unsigned int numVerts, unsigned short* indices, unsigned int numIndices, unsigned int indexingMode );
	Geometry* addGeometry( const Vertex* verts, unsigned int numVerts, unsigned int indexingMode );
	Geometry* loadGeometry( const char* path );
	Font* loadFont( const char* path );
	Shader* loadShader( const char* vertPath, const char* fragShader );
	Texture* loadTexture( const char* path );
	Texture* loadPreppedTexture( const char* path );
};

