#pragma once
#include <GL\glew.h>
#include <QtOpenGL\QGLWidget>
#include "Vertex.h"
#include "Timer.h"
#include "Qt\qtimer.h"
#include <glm\glm.hpp>
#include "Engine.h"
#include <iostream>


class ENGINE_SHARED GeneralGlWindow : public QGLWidget
{

	Q_OBJECT

	QTimer intTimer;
	Timer timer;

	static int currentGeometryIndex;


protected:
	void initializeGL();
	void paintGL();

	virtual void init() = 0;
	virtual void paint() = 0;
	virtual void update( float dt ) = 0;

public:
	enum ParameterType
	{
		// These values matter:
		PT_INT = 1,
		PT_FLOAT = sizeof(float) * 1,
		PT_VEC2 = sizeof(float) * 2,
		PT_VEC3 = sizeof(float) * 3,
		PT_VEC4 = sizeof(float) * 4,
		PT_MAT3 = sizeof(float) * 9,
		PT_MAT4 = sizeof(float) *16,
	};

	static const int NUMBER_OF_BUFFERS = 10;

	class BufferInfo
	{
	public:
		static const int SIZE = 10000000;
		int freeptr;
		GLuint id;

		inline BufferInfo();

		inline void initialize();
	} buffers[NUMBER_OF_BUFFERS];

	struct GeometryInfo
	{
		unsigned int vertexArrayID;
		unsigned int bufferId;
		unsigned int numIndices;
		unsigned int numVertices;
		unsigned int indexingMode;
		unsigned int bufferOffset;

		ushort *indices;
		Neumont::Vertex *vertices;

		inline GeometryInfo(){};
		inline GeometryInfo( unsigned int vertexArrayID, unsigned int bufferId, unsigned int numIndices, unsigned int numVertices, unsigned int indexingMode, unsigned int bufferOffset );
		inline ushort* getIndices();
		inline Neumont::Vertex* getVertices();
	} geometryInfos[100];

	struct ShaderInfo;

	struct TextureInfo
	{
		unsigned int textureID;

		inline TextureInfo(){};
		inline TextureInfo( unsigned int texture );
	} textureInfos[100];

	struct FrameBufferInfo
	{
		unsigned int frameBufferId;
		TextureInfo *colorTexture, *depthTexture;

		int width, height;

		inline FrameBufferInfo(){};
		inline FrameBufferInfo( unsigned int texture, TextureInfo* color, TextureInfo* depth, int width, int height );
	} framebufferInfos[50];

	struct UniformInfo
	{
		int location;
		const char * name;

		const float* data;

		ParameterType type;

		inline UniformInfo(){};
		inline UniformInfo( int locationIn, const char * name, const float* dataIn, ParameterType typeIn );
		inline UniformInfo( const char * name, const float* dataIn, ParameterType typeIn );

		void ENGINE_SHARED send( GeneralGlWindow::ShaderInfo * shader );

	} uniformInfos[100];

	struct ShaderInfo
	{
		unsigned int shaderProgramID, vertId, fragId;

		const char *vertexPath, *fragmentPath;
		long timeStamp;

		inline ShaderInfo();
		inline ShaderInfo( unsigned int program );

		static GLuint numGlobalUniformParameters;
		static UniformInfo * globalUniformParameters;

		GLuint numUniformParameters;
		UniformInfo uniformParameters[20];
	} shaderInfos[50];

	struct Renderable
	{
		GeometryInfo* whatGeometry;
		ShaderInfo* howShader;
		glm::mat4 where;
		bool visible;
		TextureInfo* trans;
		TextureInfo* texture;
		TextureInfo* occlusion;
		

		inline Renderable();

		inline Renderable( GeometryInfo* whatGeometry, glm::mat4 whereMatrix, ShaderInfo* howShader, TextureInfo* texture = nullptr, TextureInfo* trans = nullptr, TextureInfo* occlusion = nullptr );

		void ENGINE_SHARED draw();

	private:
		friend class GeneralGlWindow;
		GLuint numUniformParameters;
		UniformInfo uniformParameters[10];
	} renderables[10000];
public:

	inline GeneralGlWindow(){}

	void setUpAttribs( GeneralGlWindow::GeometryInfo* geo );

	template<class TVert>
	inline GeometryInfo* addGeometry( const TVert* verts, uint numVerts, ushort* indices, uint numIndices, GLuint indexingMode);

	template<class TVert>
	inline GeometryInfo* addGeometry( const TVert* verts, uint numVerts, uint* indices, uint numIndices, GLuint indexingMode);

	GeometryInfo* addGeometry( const void* verts, uint numVertices, uint sizeVerts, ushort* indices, uint numIndices, GLuint indexingMode );
	GeometryInfo* addGeometry( const void* verts, uint numVertices, uint sizeVerts, uint* indices, uint numIndices, GLuint indexingMode );
	
	ShaderInfo* addShaderInfo( const char* vertexShaderFile, const char* fragmentShaderFile);
	
	Renderable* addRenderable( GeometryInfo* whatGeometry, const glm::mat4& whereMatrix, ShaderInfo* howShaders, TextureInfo* texture = nullptr, TextureInfo* trans = nullptr, TextureInfo* occlusion = nullptr);
	Renderable* replaceRenderable( Renderable* oldRenderable, GeometryInfo* whatGeometry, const glm::mat4& whereMatrix, ShaderInfo* howShaders, TextureInfo* texture = nullptr);

	GeometryInfo* loadFile( const char* path );

	void addShaderStreamedParameter( GeometryInfo* geoID, uint layoutLocation,  GeneralGlWindow::ParameterType parameterType, uint bufferOffset, uint bufferStride);
	
	void addUniformParameter( const char* name, GeneralGlWindow::ParameterType parameterType, const float* value);
	void addUniformParameter( ShaderInfo* shader, const char* name, GeneralGlWindow::ParameterType parameterType, const float* value);
	void addUniformParameter( Renderable* renderable, const char* name, GeneralGlWindow::ParameterType parameterType, const float* value);

	void setUniformParameter( const char* name, GeneralGlWindow::ParameterType parameterType, const float* value);
	void setUniformParameter( ShaderInfo* shader, const char* name, GeneralGlWindow::ParameterType parameterType, const float* value);
	void setUniformParameter( Renderable* renderable, const char* name, GeneralGlWindow::ParameterType parameterType, const float* value);

	FrameBufferInfo* createFrameBuffer(int width, int height);

	TextureInfo* addTexture(const char* fileName);
	TextureInfo* addTexture(QImage * image);
	TextureInfo* addTexture(int width, int height, GLenum component = GL_RGBA, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);

	TextureInfo* addTextureCubeMap(const char* positiveX, const char* positiveY, const char* positiveZ, const char* negativeX, const char* negativeY, const char* negativeZ );

	void updateTexture(TextureInfo* texture, const char* fileName);
	void updateTexture(TextureInfo* texture, QImage * image);

	void setRenderTarget( FrameBufferInfo * frameBuffer );
	void resetRenderTarget();

private:
	BufferInfo* nextFreeBuffer( int size );



private slots:
	void interval();
};

#include "GeneralGLWindow.inl"