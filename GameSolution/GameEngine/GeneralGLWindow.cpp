#include <GL\glew.h>
#include <GL\GL.h>
#include <glm\ext.hpp>
#include <fstream>
#include <iostream>
#include "Vertex.h"
#include <ShapeGenerator.h>
#include "DebugMenus.h"

#include "GeneralGLWindow.h"
#include <Qt/qtimer.h>

#include "DebugShapes.h"

int GeneralGlWindow::currentGeometryIndex = 0;

GeneralGlWindow::BufferInfo* GeneralGlWindow::nextFreeBuffer( int size )
{
	BufferInfo* result = nullptr;
	bool found = false;

	for( int i = 0; i < NUMBER_OF_BUFFERS; i++ )
	{
		if(!found)
		{
			if( buffers[i].SIZE - buffers[i].freeptr )
			{
				result = &buffers[i];
				found = true;
			}
		}
	}

	return result;
}

void GeneralGlWindow::initializeGL()
{
	glewInit();

	for( int i = 0; i < NUMBER_OF_BUFFERS; i++)
	{
		buffers[i].initialize();
	}

	DebugShapes::init( this );

	init();

	timer.interval();

	connect( &intTimer, SIGNAL(timeout()), this, SLOT(interval()) );
	intTimer.start(0);
}

void GeneralGlWindow::paintGL()
{
	paint();
	DebugShapes::draw();
}

void GeneralGlWindow::interval()
{
	float dt = timer.interval();
	DebugMenus::update();
	update( dt );
	DebugShapes::update( dt );
}

void GeneralGlWindow::setUpAttribs( GeometryInfo* geo )
{
	//Position
	addShaderStreamedParameter( geo, 0, GeneralGlWindow::ParameterType::PT_VEC3, geo->bufferOffset + geo->numIndices*sizeof(GLushort) + Neumont::Vertex::POSITION_OFFSET, Neumont::Vertex::STRIDE );
	//Color
	addShaderStreamedParameter( geo, 1, GeneralGlWindow::ParameterType::PT_VEC4, geo->bufferOffset + geo->numIndices*sizeof(GLushort) + Neumont::Vertex::COLOR_OFFSET, Neumont::Vertex::STRIDE );
	//Normal
	addShaderStreamedParameter( geo, 2, GeneralGlWindow::ParameterType::PT_VEC3, geo->bufferOffset + geo->numIndices*sizeof(GLushort) + Neumont::Vertex::NORMAL_OFFSET, Neumont::Vertex::STRIDE );
	//UV
	addShaderStreamedParameter( geo, 3, GeneralGlWindow::ParameterType::PT_VEC2, geo->bufferOffset + geo->numIndices*sizeof(GLushort) + Neumont::Vertex::UV_OFFSET, Neumont::Vertex::STRIDE );
}



GeneralGlWindow::GeometryInfo* GeneralGlWindow::loadFile( const char* path )
{
	std::ifstream file(path, std::ios::in|std::ios::binary);

	char header[8];

	file.read( header, 8 );

	int vertexCount = *reinterpret_cast<int*>( header );
	int indexCount = *(reinterpret_cast<int*>( header ) + 1);

	std::cout << vertexCount << std::endl;
	std::cout << indexCount << std::endl;

	int vertsSize = (vertexCount) * sizeof(Neumont::Vertex);
	int indexSize = indexCount * sizeof(ushort);

	char* data = new char[ vertsSize + indexSize ];

	file.read( data, vertsSize + indexSize );

	ushort* indices = reinterpret_cast<ushort*>(data);
	Neumont::Vertex* verts = reinterpret_cast<Neumont::Vertex*>(data+indexSize);
	
	file.close();

	GeometryInfo* result = addGeometry( verts, vertexCount, indices, indexCount, GL_TRIANGLES );
	delete [] data;
	return result;
}

GeneralGlWindow::GeometryInfo* GeneralGlWindow::addGeometry( const void* verts, uint numVertices, uint sizeVerts, ushort* indices, uint numIndices, GLuint indexingMode )
{
	int indexsize = numIndices * sizeof( ushort );

	BufferInfo* freeBuffer = nextFreeBuffer( sizeVerts + indexsize);

	GLuint arr;
	glGenVertexArrays(1,&arr);

	glBindVertexArray(arr);
	glBindBuffer( GL_ARRAY_BUFFER, freeBuffer -> id );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, freeBuffer -> id );

	glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, freeBuffer->freeptr, indexsize, indices );
	glBufferSubData( GL_ARRAY_BUFFER, freeBuffer->freeptr + indexsize, sizeVerts, verts );

		

	std::cout << "Geometry " << GeneralGlWindow::currentGeometryIndex << " assigned to buffer " << (freeBuffer->id) << " at position " << (freeBuffer->freeptr) << ". Vert: " << sizeVerts << " Ind: " << indexsize << std::endl;

	geometryInfos[ GeneralGlWindow::currentGeometryIndex ] = GeometryInfo( arr, freeBuffer->id, numIndices, numVertices, indexingMode, freeBuffer->freeptr );
	GeneralGlWindow::currentGeometryIndex++;

	freeBuffer->freeptr += sizeVerts + indexsize;

	return &geometryInfos[ GeneralGlWindow::currentGeometryIndex-1 ];
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

GeneralGlWindow::ShaderInfo* GeneralGlWindow::addShaderInfo( const char* vertexShaderFile, const char* fragmentShaderFile)
{
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);


	std::string vertShaderStr = readFile(vertexShaderFile);
	std::string fragShaderStr = readFile(fragmentShaderFile);
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

	static int currentShaderIndex = 0;

	shaderInfos[ currentShaderIndex ] = ShaderInfo( program );
	currentShaderIndex++;

	return &shaderInfos[ currentShaderIndex-1 ];
}

GeneralGlWindow::Renderable* GeneralGlWindow::addRenderable( GeometryInfo* whatGeometry, const glm::mat4& whereMatrix, ShaderInfo* howShaders, TextureInfo* texture = nullptr)
{
	static int currentRenderableIndex = 0;

	renderables[ currentRenderableIndex ] = Renderable( whatGeometry, whereMatrix, howShaders, texture );
	currentRenderableIndex++;

	return &renderables[ currentRenderableIndex-1 ];
}

GeneralGlWindow::Renderable* GeneralGlWindow::replaceRenderable( Renderable *renderable, GeometryInfo* whatGeometry, const glm::mat4& whereMatrix, ShaderInfo* howShaders, TextureInfo* texture = nullptr)
{
	/*static int currentRenderableIndex = 0;

	renderables[ currentRenderableIndex ] = Renderable( whatGeometry, whereMatrix, howShaders, texture );
	currentRenderableIndex++;*/

	*renderable = Renderable( whatGeometry, whereMatrix, howShaders, texture );

	return renderable;
}

//void GeneralGlWindow::addShaderStreamedParameter( ShaderInfo* shaderID, uint layoutLocation, GeneralGlWindow::ParameterType parameterType, uint bufferOffset, uint bufferStride)
void GeneralGlWindow::addShaderStreamedParameter( GeometryInfo* geoID, uint layoutLocation, GeneralGlWindow::ParameterType parameterType, uint bufferOffset, uint bufferStride)
{
	glBindVertexArray( geoID->vertexArrayID );
	glEnableVertexAttribArray( layoutLocation );
	glVertexAttribPointer( layoutLocation, parameterType / sizeof(float), GL_FLOAT, GL_FALSE, bufferStride, (void*)bufferOffset );
}
	
int currentUniformIndex = 0;


void GeneralGlWindow::addUniformParameter( ShaderInfo* shader, const char* name, GeneralGlWindow::ParameterType parameterType, const float* value)
{
	GLint loc = glGetUniformLocation(shader->shaderProgramID, name);
	if ( loc != -1 )
	{
		int foundExisting = -1;
		for( int i = 0; i < shader->numUniformParameters; i++ )
		{
			if ( loc == shader->uniformParameters[i].location && parameterType == shader->uniformParameters[i].type )
				foundExisting = i;
		}
		if( foundExisting != -1 )
		{
			shader->uniformParameters[ foundExisting ].data = value;
		}
		else
		{
			shader->uniformParameters[shader->numUniformParameters] = UniformInfo( loc, value, parameterType );
			shader->numUniformParameters++;
		}
	}
}

void GeneralGlWindow::addUniformParameter( Renderable* renderable, const char* name, GeneralGlWindow::ParameterType parameterType, const float* value)
{
	GLint loc = glGetUniformLocation(renderable->howShader->shaderProgramID, name);
	if ( loc != -1 )
	{
		int foundExisting = -1;
		for( int i = 0; i < renderable->numUniformParameters; i++ )
		{
			if ( loc == renderable->uniformParameters[i].location && parameterType == renderable->uniformParameters[i].type )
				foundExisting = i;
		}
		if( foundExisting != -1 )
		{
			renderable->uniformParameters[ foundExisting ].data = value;
		}
		else
		{
			renderable->uniformParameters[renderable->numUniformParameters] = UniformInfo( loc, value, parameterType );
			renderable->numUniformParameters++;
		}
	}
}


void GeneralGlWindow::setUniformParameter( ShaderInfo* shader, const char* name, GeneralGlWindow::ParameterType parameterType, const float* value)
{
	glUseProgram( shader->shaderProgramID );
	GLint loc = glGetUniformLocation(shader->shaderProgramID, name);
	UniformInfo inf = UniformInfo( loc, value, parameterType );
	inf.send();
}

void GeneralGlWindow::setUniformParameter( Renderable* renderable, const char* name, GeneralGlWindow::ParameterType parameterType, const float* value)
{
	setUniformParameter( renderable->howShader, name, parameterType, value );
}

GeneralGlWindow::TextureInfo* GeneralGlWindow::addTexture(const char* fileName)
{
	static QImage image;
	image.load( fileName );

	image = convertToGLFormat( image );

	unsigned int texture;
	glGenTextures( 1, &texture );
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
	
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	static int currentTextureIndex = 0;

	textureInfos[ currentTextureIndex ] = TextureInfo( texture );
	currentTextureIndex++;

	return &textureInfos[ currentTextureIndex-1 ];
}

void GeneralGlWindow::UniformInfo::send()
{
	if (location != -1)
	{
		switch( type )
		{
		case GeneralGlWindow::ParameterType::PT_FLOAT:
			glUniform1f( location, *data );
			break;

		case GeneralGlWindow::ParameterType::PT_VEC2:
			glUniform2fv( location, 1, data );
			break;
		case GeneralGlWindow::ParameterType::PT_VEC3:
			glUniform3fv( location, 1, data );
			break;
		case GeneralGlWindow::ParameterType::PT_VEC4:
			glUniform4fv( location, 1, data );
			break;

		case GeneralGlWindow::ParameterType::PT_MAT3:
			glUniformMatrix3fv( location, 1, false, data );
			break;
		case GeneralGlWindow::ParameterType::PT_MAT4:
			glUniformMatrix4fv( location, 1, false, data );
			break;
		}
	}
}

void GeneralGlWindow::Renderable::draw()
{
	if (visible)
	{
		glUseProgram( howShader->shaderProgramID );

		GLint loc = glGetUniformLocation( howShader->shaderProgramID, "useTexture");

		if( texture != NULL )
		{
			if (loc != -1)
				glUniform1i(loc, 1);

			glActiveTexture( GL_TEXTURE0 );

			glBindTexture( GL_TEXTURE_2D, texture->textureID );

			loc = glGetUniformLocation( howShader->shaderProgramID, "tex");
			glUniform1i(loc, 0);
		}
		else
		{
			if (loc != -1)
				glUniform1i(loc, 0);
		}

		loc = glGetUniformLocation( howShader->shaderProgramID, "useLighting");
		if( loc != -1 )
		{
			if( whatGeometry->indexingMode != GL_LINES )
			{
				glUniform1i(loc, 1);
			}
			else
			{
				glUniform1i(loc, 0);
			}
		}

		loc = glGetUniformLocation( howShader->shaderProgramID, "world");
		glUniformMatrix4fv(loc, 1, false, (float*)&where );


		for( int i = 0; i < howShader->numUniformParameters; i++ )
		{
			howShader->uniformParameters[i].send();
		}

		for( int i = 0; i < numUniformParameters; i++ )
		{
			uniformParameters[i].send();
		}
				
		glBindVertexArray( whatGeometry->vertexArrayID );
		glDrawElements( whatGeometry->indexingMode, whatGeometry->numIndices, GL_UNSIGNED_SHORT, (void*)whatGeometry->bufferOffset );
	}
}