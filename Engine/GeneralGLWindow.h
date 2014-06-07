#pragma once
#include <GL\glew.h>
#include <QtOpenGL\QGLWidget>
#include "Vertex.h"
#include <glm\glm.hpp>
#include "Timer.h"
#include "Qt\qtimer.h"


class GeneralGlWindow : public QGLWidget
{

	Q_OBJECT

	QTimer intTimer;
	Timer timer;

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
		PT_FLOAT = sizeof(float) * 1,
		PT_VEC2 = sizeof(float) * 2,
		PT_VEC3 = sizeof(float) * 3,
		PT_VEC4 = sizeof(float) * 4,
		PT_MAT3 = sizeof(float) * 9,
		PT_MAT4 = sizeof(float) *16,
	};

	struct GeometryInfo
	{
		unsigned int vertexArrayID;
		unsigned int numIndices;
		unsigned int indexingMode;
		unsigned int bufferOffset;


		inline GeometryInfo(){};
		inline GeometryInfo( unsigned int vertexArrayID, unsigned int numIndices, unsigned int indexingMode, unsigned int bufferOffset ) 
		{
			this->vertexArrayID = vertexArrayID;
			this->numIndices = numIndices;
			this->indexingMode = indexingMode;
			this->bufferOffset = bufferOffset;
		}
	} geometryInfos[10];

	struct TextureInfo
	{
		unsigned int textureID;

		inline TextureInfo(){};
		inline TextureInfo( unsigned int texture ){ textureID = texture; }
	} textureInfos[10];

	struct UniformInfo
	{
		int location;

		const float* data;

		ParameterType type;

		inline UniformInfo(){};
		inline UniformInfo( int locationIn, const float* dataIn, ParameterType typeIn )
		{
			location = locationIn;
			data = dataIn;
			type = typeIn;
		}

		inline void send()
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
	} uniformInfos[100];

	struct ShaderInfo
	{
		unsigned int shaderProgramID;

		inline ShaderInfo(){ numUniformParameters = 0; };
		inline ShaderInfo( unsigned int program ){ shaderProgramID = program; numUniformParameters = 0; }

		GLuint numUniformParameters;
		UniformInfo uniformParameters[20];
	} shaderInfos[10];

	struct Renderable
	{
		GeometryInfo* whatGeometry;
		ShaderInfo* howShader;
		glm::mat4 where;
		bool visible;
		TextureInfo* texture;

		inline Renderable()
		{
			numUniformParameters = 0;
			visible = true;
		}

		inline Renderable( GeometryInfo* whatGeometry, glm::mat4 whereMatrix, ShaderInfo* howShader, TextureInfo* texture = nullptr ) 
		{ 
			numUniformParameters = 0;
			this->whatGeometry = whatGeometry;
			where = whereMatrix;
			this->howShader = howShader;
			this->texture = texture;
			visible = true;
		}

		inline void draw()
		{
			if (visible)
			{
				glUseProgram( howShader->shaderProgramID );

				glActiveTexture( GL_TEXTURE0 );

				glBindTexture( GL_TEXTURE_2D, texture->textureID );

				GLint loc = glGetUniformLocation( howShader->shaderProgramID, "tex");
				glUniform1i(loc, 0);

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
	private:
		friend class GeneralGlWindow;
		GLuint numUniformParameters;
		UniformInfo uniformParameters[10];
	} renderables[10];
public:

	inline GeneralGlWindow()
	{
	}

	void setUpAttribs( GeneralGlWindow::GeometryInfo* geo );

	template<class TVert>
	GeometryInfo* addGeometry( const TVert* verts, uint numVerts, ushort* indices, uint numIndices, GLuint indexingMode);
	
	ShaderInfo* addShaderInfo( const char* vertexShaderFile, const char* fragmentShaderFile);
	
	Renderable* addRenderable( GeometryInfo* whatGeometry, const glm::mat4& whereMatrix, ShaderInfo* howShaders, TextureInfo* texture);

	GeometryInfo* loadFile( const char* path );

	//void addShaderStreamedParameter( ShaderInfo* shaderID, uint layoutLocation,  GeneralGlWindow::ParameterType parameterType, uint bufferOffset, uint bufferStride);
	void addShaderStreamedParameter( GeometryInfo* geoID, uint layoutLocation,  GeneralGlWindow::ParameterType parameterType, uint bufferOffset, uint bufferStride);
	
	void addUniformParameter( ShaderInfo* shader, const char* name, GeneralGlWindow::ParameterType parameterType, const float* value);
	void addUniformParameter( Renderable* renderable, const char* name, GeneralGlWindow::ParameterType parameterType, const float* value);

	void setUniformParameter( ShaderInfo* shader, const char* name, GeneralGlWindow::ParameterType parameterType, const float* value);
	void setUniformParameter( Renderable* renderable, const char* name, GeneralGlWindow::ParameterType parameterType, const float* value);

	TextureInfo* addTexture(const char* fileName);

private slots:
	void interval();
};
