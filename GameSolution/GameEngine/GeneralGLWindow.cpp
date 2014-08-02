#include <GL\glew.h>
#include <GL\GL.h>
#include <glm\ext.hpp>
#include <fstream>
#include <iostream>
#include "Vertex.h"
#include <ShapeGenerator.h>
#include "DebugMenus.h"

#include <sys/types.h>
#include <sys/stat.h> 

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
			if( buffers[i].SIZE - buffers[i].freeptr > size )
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

void calculateTangents(Neumont::Vertex * verts, int vertexCount, ushort* indices, int indexCount)
{
	glm::vec3 *tan1 = new glm::vec3[vertexCount * 2];
    glm::vec3 *tan2 = tan1 + vertexCount;
    ZeroMemory(tan1, vertexCount * sizeof(glm::vec3) * 2);
    
	for (long a = 0; a < indexCount/3; a++)
    {
		int i1 = indices[a*3+0];
        int i2 = indices[a*3+1];
        int i3 = indices[a*3+2];
        
		const Neumont::Vertex& v1 = verts[i1];
        const Neumont::Vertex& v2 = verts[i2];
        const Neumont::Vertex& v3 = verts[i3];
        
        /*const Point2D& w1 = texcoord[i1];
        const Point2D& w2 = texcoord[i2];
        const Point2D& w3 = texcoord[i3];*/
        
        float x1 = v2.normal.x - v1.normal.x;
        float x2 = v3.normal.x - v1.normal.x;
        float y1 = v2.normal.y - v1.normal.y;
        float y2 = v3.normal.y - v1.normal.y;
        float z1 = v2.normal.z - v1.normal.z;
        float z2 = v3.normal.z - v1.normal.z;
        
		float s1 = v2.uv.x - v1.uv.x;
        float s2 = v3.uv.x - v1.uv.x;
        float t1 = v2.uv.y - v1.uv.y;
        float t2 = v3.uv.y - v1.uv.y;
        
        float r = 1.0F / (s1 * t2 - s2 * t1);
        glm::vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
                (t2 * z1 - t1 * z2) * r);
        glm::vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
                (s1 * z2 - s2 * z1) * r);
        
        tan1[i1] += sdir;
        tan1[i2] += sdir;
        tan1[i3] += sdir;
        
        tan2[i1] += tdir;
        tan2[i2] += tdir;
        tan2[i3] += tdir;
    }
    
    for (long a = 0; a < vertexCount; a++)
    {
		const glm::vec3& n = verts[a].normal;
        const glm::vec3& t = tan1[a];
        
        // Gram-Schmidt orthogonalize
		verts[a].color = glm::vec4( glm::normalize(t - n * glm::dot(n, t)), 0 );
        
        // Calculate handedness
		verts[a].color.w = (glm::dot(glm::cross(n, t), tan2[a]) < 0.0F) ? -1.0F : 1.0F;
		//std::cout << shapeData->verts[a].normal.x << "; " << shapeData->verts[a].normal.y << "; " << shapeData->verts[a].normal.z << ";;; " << shapeData->verts[a].color.x << "; " << shapeData->verts[a].color.y << "; " << shapeData->verts[a].color.z << "; " << shapeData->verts[a].color.w << std::endl;
    }
    
    delete[] tan1;
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
	//int indexSize = indexCount * sizeof(uint);

	char* data = new char[ vertsSize + indexSize ];

	file.read( data, vertsSize + indexSize );

	ushort* indices = reinterpret_cast<ushort*>(data);
	//uint* indices = reinterpret_cast<uint*>(data);
	Neumont::Vertex* verts = reinterpret_cast<Neumont::Vertex*>(data+indexSize);

	for( int i = 0; i < 3; i++ )
	{
		std::cout << "Normal: " << verts[i].normal.x << ", " << verts[i].normal.y << ", " << verts[i].normal.z<<std::endl << "  Tangent: " << verts[i].color.x << ", " << verts[i].color.y << ", " << verts[i].color.z << ", " << verts[i].color.w  <<std::endl<<std::endl;
	}
	calculateTangents( verts, vertexCount, indices, indexCount );
	std::cout << "change" << std::endl;
	for( int i = 0; i < 3; i++ )
	{
		std::cout << "Normal: " << verts[i].normal.x << ", " << verts[i].normal.y << ", " << verts[i].normal.z<<std::endl << "  Tangent: " << verts[i].color.x << ", " << verts[i].color.y << ", " << verts[i].color.z << ", " << verts[i].color.w  <<std::endl<<std::endl;
	}

	for( int i = 0; i < indexCount / 3; i++ )
	{
		//std::cout << indices[i*3] << ", " << indices[i*3+1] << ", " << indices[i*3+2] << std::endl;
	}
	
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

		

	std::cout << "Geometry " << GeneralGlWindow::currentGeometryIndex << " assigned to buffer " << (freeBuffer->id) << " at position " << (freeBuffer->freeptr) << ". Vert: "<< sizeVerts << " in " << numVertices<< " Ind: " << indexsize << " in " << numIndices << std::endl;

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

void loadShaderData( GeneralGlWindow::ShaderInfo * shader )
{
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vertShaderStr = readFile(shader->vertexPath);
	std::string fragShaderStr = readFile(shader->fragmentPath);

	struct stat st;
	long lastUpdated;

    stat(shader->vertexPath, &st);
	lastUpdated = st.st_mtime;
	stat(shader->fragmentPath, &st);
	if( lastUpdated < st.st_mtime )
	{
		lastUpdated = st.st_mtime;
	}

	shader->timeStamp = lastUpdated;

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
	GLuint program;
	if( shader->shaderProgramID == -1 )
	{
		program = glCreateProgram();
	}
	else
	{
		program = shader->shaderProgramID;
		glDetachShader(program, shader->vertId );
		glDetachShader(program, shader->fragId );
	}
	
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	
	glLinkProgram(program);

	shader->vertId = vertShader;
	shader->fragId = fragShader;

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

	shader->shaderProgramID = program;

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
}

int currentShaderIndex = 0;

GeneralGlWindow::ShaderInfo* GeneralGlWindow::addShaderInfo( const char* vertexShaderFile, const char* fragmentShaderFile)
{
	shaderInfos[ currentShaderIndex ] = ShaderInfo();
	shaderInfos[ currentShaderIndex ].vertexPath = vertexShaderFile;
	shaderInfos[ currentShaderIndex ].fragmentPath = fragmentShaderFile;

	loadShaderData( &shaderInfos[ currentShaderIndex ] );

	currentShaderIndex++;

	return &shaderInfos[ currentShaderIndex-1 ];
}

GeneralGlWindow::Renderable* GeneralGlWindow::addRenderable( GeometryInfo* whatGeometry, const glm::mat4& whereMatrix, ShaderInfo* howShaders, TextureInfo* texture, TextureInfo* trans, TextureInfo* occlusion )
{
	static int currentRenderableIndex = 0;

	renderables[ currentRenderableIndex ] = Renderable( whatGeometry, whereMatrix, howShaders, texture, trans, occlusion );
	currentRenderableIndex++;

	return &renderables[ currentRenderableIndex-1 ];
}

GeneralGlWindow::Renderable* GeneralGlWindow::replaceRenderable( Renderable *renderable, GeometryInfo* whatGeometry, const glm::mat4& whereMatrix, ShaderInfo* howShaders, TextureInfo* texture )
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

GeneralGlWindow::UniformInfo * GeneralGlWindow::ShaderInfo::globalUniformParameters = new GeneralGlWindow::UniformInfo[10];
GLuint GeneralGlWindow::ShaderInfo::numGlobalUniformParameters = 0;

void GeneralGlWindow::addUniformParameter( const char* name, GeneralGlWindow::ParameterType parameterType, const float* value)
{
	GeneralGlWindow::ShaderInfo::globalUniformParameters[ GeneralGlWindow::ShaderInfo::numGlobalUniformParameters++ ] = UniformInfo( name, value, parameterType );
}

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
			shader->uniformParameters[shader->numUniformParameters] = UniformInfo( loc, name, value, parameterType );
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
			renderable->uniformParameters[renderable->numUniformParameters] = UniformInfo( loc, name, value, parameterType );
			renderable->numUniformParameters++;
		}
	}
}


void GeneralGlWindow::setUniformParameter( const char* name, GeneralGlWindow::ParameterType parameterType, const float* value)
{
	for( int i = 0; i < currentShaderIndex; i++ )
	{
		setUniformParameter( &shaderInfos[i], name, parameterType, value );
	}
}

void GeneralGlWindow::setUniformParameter( ShaderInfo* shader, const char* name, GeneralGlWindow::ParameterType parameterType, const float* value)
{
	glUseProgram( shader->shaderProgramID );
	GLint loc = glGetUniformLocation(shader->shaderProgramID, name);
	UniformInfo inf = UniformInfo( loc, name, value, parameterType );
	inf.send(shader);
}

void GeneralGlWindow::setUniformParameter( Renderable* renderable, const char* name, GeneralGlWindow::ParameterType parameterType, const float* value)
{
	setUniformParameter( renderable->howShader, name, parameterType, value );
}

GeneralGlWindow::FrameBufferInfo* GeneralGlWindow::createFrameBuffer(int width, int height)
{
	
	TextureInfo * col = addTexture( width, height );
	TextureInfo * depth = addTexture( width, height, GL_DEPTH_COMPONENT32, GL_DEPTH_COMPONENT, GL_FLOAT );
	

	static int currentFrameBufferIndex = 0;

	unsigned int frameBufferId;

	int errorPre = glGetError();

	glActiveTexture( GL_TEXTURE0 );
	int errorOne = glGetError();
	glGenFramebuffers( 1, &frameBufferId );
	int errorTwo = glGetError();
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, frameBufferId );
	int errorThree = glGetError();

	glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, col->textureID, 0 );
	glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth->textureID, 0 );

	int errorFour = glGetError();

	unsigned int status = glCheckFramebufferStatus( GL_DRAW_FRAMEBUFFER );
	if( status != GL_FRAMEBUFFER_COMPLETE )
	{
		int error = glGetError();
		std::cout << "NO FRAME BUFFER, BRO" << std::endl;
	}

	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );

	framebufferInfos[currentFrameBufferIndex] = FrameBufferInfo( frameBufferId, col, depth, width, height );

	currentFrameBufferIndex++;

	return &framebufferInfos[currentFrameBufferIndex - 1];
}

int currentTextureIndex = 0;

GeneralGlWindow::TextureInfo* GeneralGlWindow::addTexture(const char* fileName)
{
	static QImage image;
	image.load( fileName );
	return addTexture( &image );
}

GeneralGlWindow::TextureInfo* GeneralGlWindow::addTexture(QImage * inputImage)
{
	unsigned int texture;
	glGenTextures( 1, &texture );

	textureInfos[ currentTextureIndex ] = TextureInfo( texture );
	currentTextureIndex++;

	updateTexture( &textureInfos[ currentTextureIndex-1 ], inputImage );

	return &textureInfos[ currentTextureIndex-1 ];
}

GeneralGlWindow::TextureInfo* GeneralGlWindow::addTexture(int width, int height, GLenum component, GLenum format, GLenum type)
{
	unsigned int texture;
	glGenTextures( 1, &texture );

	textureInfos[ currentTextureIndex ] = TextureInfo( texture );
	currentTextureIndex++;

	glBindTexture( GL_TEXTURE_2D, texture );
	glTexImage2D(GL_TEXTURE_2D, 0, component, width, height, 0, format, type, 0);
	
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	return &textureInfos[ currentTextureIndex-1 ];
}

GeneralGlWindow::TextureInfo* GeneralGlWindow::addTextureCubeMap(const char* positiveX, const char* positiveY, const char* positiveZ, const char* negativeX, const char* negativeY, const char* negativeZ )
{
	unsigned int texture;
	glGenTextures( 1, &texture );

	textureInfos[ currentTextureIndex ] = TextureInfo( texture );
	currentTextureIndex++;

	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	QImage image;

	image.load( positiveX );
	image = convertToGLFormat( image );
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

	image.load( positiveY );
	image = convertToGLFormat( image );
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

	image.load( positiveZ );
	image = convertToGLFormat( image );
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
	
	image.load( negativeX );
	image = convertToGLFormat( image );
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

	image.load( negativeY );
	image = convertToGLFormat( image );
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

	image.load( negativeZ );
	image = convertToGLFormat( image );
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
	
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	return &textureInfos[ currentTextureIndex-1 ];
}

void GeneralGlWindow::updateTexture(TextureInfo* texture, const char* fileName)
{
	static QImage image;
	image.load( fileName );
	return updateTexture( texture, &image );
}

void GeneralGlWindow::updateTexture(TextureInfo* texture, QImage * inputImage)
{
	QImage image = convertToGLFormat( *inputImage );

	glBindTexture( GL_TEXTURE_2D, texture->textureID );
	glTexImage2D(GL_TEXTURE_2D, 0, 3, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
	
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
}

void GeneralGlWindow::UniformInfo::send( GeneralGlWindow::ShaderInfo * shader )
{
	int loc = location;
	if( location == -1 )
	{
		loc = glGetUniformLocation( shader->shaderProgramID, name);
		if( loc != -1 )
		{
			//std::cout << "Found Global: " << shader->fragmentPath << " uni " << name << std::endl;
		}
	}
	if (loc != -1)
	{
		switch( type )
		{
		case GeneralGlWindow::ParameterType::PT_INT:
			glUniform1i( loc, *((int*)data) );
			break;

		case GeneralGlWindow::ParameterType::PT_FLOAT:
			glUniform1f( loc, *data );
			break;

		case GeneralGlWindow::ParameterType::PT_VEC2:
			glUniform2fv( loc, 1, data );
			break;
		case GeneralGlWindow::ParameterType::PT_VEC3:
			glUniform3fv( loc, 1, data );
			break;
		case GeneralGlWindow::ParameterType::PT_VEC4:
			glUniform4fv( loc, 1, data );
			break;

		case GeneralGlWindow::ParameterType::PT_MAT3:
			glUniformMatrix3fv( loc, 1, false, data );
			break;
		case GeneralGlWindow::ParameterType::PT_MAT4:
			glUniformMatrix4fv( loc, 1, false, data );
			break;
		}
	}
}

void GeneralGlWindow::Renderable::draw()
{
	if (visible)
	{
		struct stat st;
		long lastUpdated;

		stat(howShader->vertexPath, &st);
		lastUpdated = st.st_mtime;
		stat(howShader->fragmentPath, &st);
		if( lastUpdated < st.st_mtime )
		{
			lastUpdated = st.st_mtime;
		}

		if( lastUpdated > howShader->timeStamp )
		{
			Sleep( 20 );
			loadShaderData( howShader );
		}

		glUseProgram( howShader->shaderProgramID );

		GLint loc;

		//Color Map
		loc = glGetUniformLocation( howShader->shaderProgramID, "useTexture");

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


		//Transparency Map
		loc = glGetUniformLocation( howShader->shaderProgramID, "useTransMap");

		if( trans != NULL )
		{
			if (loc != -1)
				glUniform1i(loc, 1);

			glActiveTexture( GL_TEXTURE1 );

			glBindTexture( GL_TEXTURE_2D, trans->textureID );
			glBindTexture( GL_TEXTURE_CUBE_MAP, trans->textureID );

			loc = glGetUniformLocation( howShader->shaderProgramID, "trans");
			glUniform1i(loc, 1);
		}
		else
		{
			if (loc != -1)
				glUniform1i(loc, 0);
		}

		//AmbientOcclusion
		loc = glGetUniformLocation( howShader->shaderProgramID, "useAmbientOcclusion");

		if( occlusion != NULL )
		{
			if (loc != -1)
				glUniform1i(loc, 1);

			glActiveTexture( GL_TEXTURE2 );

			glBindTexture( GL_TEXTURE_2D, occlusion->textureID );

			loc = glGetUniformLocation( howShader->shaderProgramID, "amo");
			glUniform1i(loc, 2);
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

		for( int i = 0; i < GeneralGlWindow::ShaderInfo::numGlobalUniformParameters; i++ )
		{
			GeneralGlWindow::ShaderInfo::globalUniformParameters[i].send(howShader);
		}

		for( int i = 0; i < howShader->numUniformParameters; i++ )
		{
			howShader->uniformParameters[i].send(howShader);
		}

		for( int i = 0; i < numUniformParameters; i++ )
		{
			uniformParameters[i].send(howShader);
		}
				
		glBindVertexArray( whatGeometry->vertexArrayID );
		glDrawElements( whatGeometry->indexingMode, whatGeometry->numIndices, GL_UNSIGNED_SHORT, (void*)whatGeometry->bufferOffset );
	}
}

void GeneralGlWindow::setRenderTarget( GeneralGlWindow::FrameBufferInfo * fb )
{
	glViewport( 0, 0, fb->width, fb->height );
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, fb->frameBufferId );
}

void GeneralGlWindow::resetRenderTarget()
{
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
}