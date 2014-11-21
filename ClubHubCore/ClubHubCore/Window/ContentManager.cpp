#include "ContentManager.h"
#include <map>
#include <fstream>
#include "Renderer\Vertex.h"
#include <GL\glew.h>
#include <iostream>
#include <string>
#include <QtGui\qimage.h>
#include <QtOpenGL\QGLWidget>
#include <glm\glm.hpp>
#include "Renderer\Texture.h"
#include "Renderer\Shader.h"
#include "Window\Font.h"
#include "Window\Character.h"
#include "Window\Kerning.h"

ContentManager::ContentManager()
{
	currentGeometry = 0;
	shaders = std::map<std::string, Shader*>();
	textures = std::map<std::string, Texture*>();
	fonts = std::map<std::string, Font*>();
}

ContentManager::~ContentManager()
{
	for( auto i = fonts.begin(); i != fonts.end(); i++ )
	{
		delete i->second;
	}
}

Buffer* ContentManager::nextFreeBuffer( unsigned int size )
{
	Buffer* result = nullptr;

	bool found = false;

	for( int i = 0; i < NUMBER_OF_BUFFERS; i++ )
	{
		if(!found)
		{
			if( buffers[i].SIZE - buffers[i].freeptr > size )
			{
				result = &buffers[i];
				found = true;
			}
		}
	}

	return result;
}

Geometry* ContentManager::addGeometry( const Vertex* verts, uint numVerts, GLuint indexingMode )
{
	ushort* indices = new ushort[numVerts];

	for( unsigned int i = 0; i < numVerts; i++ )
	{
		indices[i] = i;
	}

	Geometry* result = addGeometry( verts, numVerts, indices, numVerts, indexingMode );

	delete indices;
	
	return result;
}

Geometry* ContentManager::addGeometry( const Vertex* verts, uint numVerts, ushort* indices, uint numIndices, GLuint indexingMode )
{
	static int currentGeometryIndex = 0;

	int sizeVerts = numVerts * sizeof( Vertex );
	int indexsize = numIndices * sizeof( ushort );

	Buffer* freeBuffer = nextFreeBuffer( sizeVerts + indexsize);

	GLuint arr;
	glGenVertexArrays(1,&arr);

	glBindVertexArray(arr);
	glBindBuffer( GL_ARRAY_BUFFER, freeBuffer -> id );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, freeBuffer -> id );

	glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, freeBuffer->freeptr, indexsize, indices );
	glBufferSubData( GL_ARRAY_BUFFER, freeBuffer->freeptr + indexsize, sizeVerts, verts );

		

	std::cout << "Geometry " << currentGeometryIndex << " assigned to buffer " << (freeBuffer->id) << " at position " << (freeBuffer->freeptr) << ". Vert: " << sizeVerts << " Ind: " << indexsize << std::endl;

	geos[ currentGeometryIndex ].vertexArrayID = arr;
	geos[ currentGeometryIndex ].numIndices = numIndices;
	geos[ currentGeometryIndex ].indexingMode = indexingMode;
	geos[ currentGeometryIndex ].bufferOffset = freeBuffer->freeptr;
	//Geometry( arr, numIndices, indexingMode, freeBuffer->freeptr );
	currentGeometryIndex++;

	freeBuffer->freeptr += sizeVerts + indexsize;

	return &geos[ currentGeometryIndex-1 ];
}

Geometry* ContentManager::geometryFromFile( const char* path )
{
	std::ifstream file(path, std::ios::in|std::ios::binary);

	char header[8];

	file.read( header, 8 );

	int vertexCount = *reinterpret_cast<int*>( header );
	int indexCount = *(reinterpret_cast<int*>( header ) + 1);

	int vertsSize = (vertexCount) * sizeof(Vertex);
	int indexSize = indexCount * sizeof(unsigned short);

	char* data = new char[ vertsSize + indexSize ];

	file.read( data, vertsSize + indexSize );

	unsigned short* indices = reinterpret_cast<unsigned short*>(data);
	Vertex* verts = reinterpret_cast<Vertex*>(data+indexSize);
	
	file.close();

	Geometry* result = addGeometry( verts, vertexCount, indices, indexCount, GL_TRIANGLES );
	delete [] data;
	Vertex::setAttributes( result );
	return result;
}

Geometry* ContentManager::loadGeometry( const char* path )
{
	return geometryFromFile( path );
}

Font* fontFromFile( const char* path )
{
	std::ifstream file(path, std::ios::in|std::ios::binary);

	char header[10];

	Font* result = new Font;

	file.read( header, 10 );
		result -> lineHeight = *reinterpret_cast<byte*>( header );
		result -> baseHeight = *reinterpret_cast<byte*>( header+1 );
		result -> width = *reinterpret_cast<unsigned short*>( header+2 );
		result -> height = *reinterpret_cast<unsigned short*>( header+4 );
		result -> minChar = *reinterpret_cast<byte*>( header+6 );
		result -> maxChar = *reinterpret_cast<byte*>( header+7 );
		result -> chars = *reinterpret_cast<byte*>( header+8 );
		result -> kerns = *reinterpret_cast<byte*>( header+9 );

		result -> characterPositions = new byte[ (result->maxChar-result->minChar) + 1 ];
		result -> characters = new Character[ result->chars ];
		result -> kernings = new Kerning[ result->kerns ];

		file.read( reinterpret_cast<char*>(result->characterPositions), sizeof(byte)*(result->maxChar-result->minChar+1) );
		file.read( reinterpret_cast<char*>(result->characters), sizeof(Character)*result->chars );
		file.read( reinterpret_cast<char*>(result->kernings), sizeof(Kerning)*result->kerns );

	file.close();

	return result;
}

Font* ContentManager::loadFont( const char* path )
{
	Font* result;
	std::string s = std::string(path);
	
	// check if key is present
	if (fonts.find(s) != fonts.end())
		result = fonts[s];
	else
	{
		result = fontFromFile( path );
		std::cout << s.substr(0,s.find_last_of('.')).append( ".glt" ).c_str();
		result->texture = loadPreppedTexture( s.substr(0,s.find_last_of('.')).append( "_0.glt" ).c_str() );
		fonts[s] = result;
	}

	return result;
}

std::string readFile(const char *filePath) 
{
	std::string content = "";
	std::ifstream fileStream(filePath, std::ios::in);

	if(!fileStream.is_open()) 
	{
		std::cout << "The file does not exist: " << filePath  << std::endl;
	}
	else
	{
		std::string line = "";
		while(!fileStream.eof()) 
		{
			std::getline(fileStream, line);
			content.append(line + "\n");
		}

		fileStream.close();
	}
	return content;
}

unsigned int shaderFromFile( const char* vertPath, const char* fragPath )
{
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);


	std::string vertShaderStr = readFile(vertPath);
	std::string fragShaderStr = readFile(fragPath);
	const char *vertShaderSrc = vertShaderStr.c_str();
	const char *fragShaderSrc = fragShaderStr.c_str();

	GLint successful = GL_FALSE;
	int logLength;

	// Compile vertex shader
	std::cout << "Compiling vertex shader." << std::endl;
	glShaderSource(vertShader, 1, &vertShaderSrc, NULL);
	glCompileShader(vertShader);

	// Check vertex shader
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &successful);
	if( successful )
	{
		std::cout << "Vertex shader successfully compiled!" << std::endl;
	}
	else
	{
		std::cout << "Vertex shader was not compiled..." << std::endl;
		glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLength);
		char* vertShaderError = new char[ logLength ];
		glGetShaderInfoLog(vertShader, logLength, NULL, &vertShaderError[0]);
		std::cout << &vertShaderError[0] << std::endl;
		delete vertShaderError;
	}

	std::cout << std::endl;

	// Compile fragment shader
	std::cout << "Compiling fragment shader." << std::endl;
	glShaderSource(fragShader, 1, &fragShaderSrc, NULL);
	glCompileShader(fragShader);

	// Check fragment shader
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &successful);
	if( successful )
	{
		std::cout << "Fragment shader successfully compiled!" << std::endl;
	}
	else
	{
		std::cout << "Fragment shader was not compiled..." << std::endl;
		glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
		char* fragShaderError = new char[ logLength ];
		glGetShaderInfoLog(fragShader, logLength, NULL, &fragShaderError[0]);
		std::cout << &fragShaderError[0] << std::endl;
		delete fragShaderError;
	}

	std::cout << std::endl;

	std::cout << "Linking program" << std::endl;
	GLuint program = glCreateProgram();
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &successful);
	if( successful )
	{
		std::cout << "Program linked successfully!" << std::endl;
	}
	else
	{
		std::cout << "Shader program was not linked..." << std::endl << std::endl;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		//std::vector<char> programError( (logLength > 1) ? logLength : 1 );
		char* programError = new char[ logLength ];
		glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
		std::cout << &programError[0] << std::endl;
		delete programError;
	}

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return program;
}

Shader* ContentManager::loadShader( const char* vertPath, const char* fragShader )
{
	Shader* result;
	std::string s = std::string(vertPath).append( fragShader );
	
	// check if key is present
	if (shaders.find(s) != shaders.end())
		result = shaders[s];
	else
	{
		result = Shader::nextFreeShader();
		result->shaderProgramID = shaderFromFile( vertPath, fragShader );
		shaders[s] = result;
	}

	return result;
}

Texture* textureFromFile( const char* path )
{
	static QImage image;
	image.load( path );

	image = QGLWidget::convertToGLFormat( image );

	unsigned int texture;
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
	glTexEnvf(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	Texture* result = Texture::nextFreeTexture();
	result -> textureID = texture;
	return result;
}

Texture* ContentManager::loadTexture( const char* path )
{
	Texture* result;
	std::string s = std::string(path);
	
	// check if key is present
	if (textures.find(s) != textures.end())
		result = textures[s];
	else
	{
		result = textureFromFile( path );
		textures[s] = result;
	}

	return result;
}

Texture* ContentManager::loadPreppedTexture( const char* path )
{
	Texture* result;
	std::string s = std::string(path);
	
	// check if key is present
	if (textures.find(s) != textures.end())
		result = textures[s];
	else
	{
		static std::ifstream file;
		file.open( path, std::ios::in | std::ios::binary );
		int width, height;
		file.read( reinterpret_cast<char*>( &width ), sizeof( width ) );
		file.read( reinterpret_cast<char*>( &height ), sizeof( height ) );
		uchar * imageData = new uchar[ width*height*4 ];
		file.read( reinterpret_cast<char*>( imageData ), width*height*4 );

		file.close();
		unsigned int texture;
		glGenTextures( 1, &texture );
		glBindTexture( GL_TEXTURE_2D, texture );
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		glTexEnvf(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_MODULATE);

		delete imageData;
	
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		result = Texture::nextFreeTexture();
		result -> textureID = texture;

		textures[s] = result;
	}

	return result;
}