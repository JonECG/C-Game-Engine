#include "Graphics.h"
#include <glm\glm.hpp>
#include "Renderable.h"
#include <GL\glew.h>
#include "ContentManager.h"
#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include <glm\gtc\matrix_transform.hpp>

#include "Font.h"
#include "Character.h"
#include "Kerning.h"
#include "CoordinateTransform.h"


Graphics::Graphics( ContentManager* content, CoordinateTransform *transform )
{
	currentTransform = new glm::mat4();
	currentColor = new glm::vec4();
	drawTransform = new glm::mat4();
	drawColor = new glm::vec4(1,1,1,1);
	this->coordinateTransform = transform;

	white = content->loadTexture( "Resources/white.png" );
	defaultFont = content->loadFont( "Resources/Tahoma.bfnt" );
	currentFont = defaultFont;

	Vertex verts[5];
	verts[0].setPosition( glm::vec3( 0, 0, 0 ) );
	verts[1].setPosition( glm::vec3( 1, 0, 0 ) );
	verts[2].setPosition( glm::vec3( 0, 1, 0 ) );
	verts[3].setPosition( glm::vec3( 1, 1, 0 ) );
	verts[4].setPosition( glm::vec3( 0, 0, 0 ) );

	for( int i = 0; i < 5; i++ )
	{
		verts[i].setColor( glm::vec4( 1, 1, 1, 1 ) );
		verts[i].setNormal( glm::vec3( 0,0,1 ) );
		verts[i].setUv( glm::vec2( verts[i].getPosition() ) );
	}

	Geometry* solidRectGeo = content->addGeometry( verts, 4, GL_TRIANGLE_STRIP );

	Vertex buff = verts[2];
	verts[2] = verts[3];
	verts[3] = buff;

	Geometry* emptyRectGeo = content->addGeometry( verts, 5, GL_LINE_STRIP );

	Vertex::setAttributes( solidRectGeo );
	Vertex::setAttributes( emptyRectGeo );

	graphicsShader = content->loadShader( "Resources/graphix.vert", "Resources/graphix.frag" );

	graphicsShader->addUniformParameter( "world", ParameterType::PT_MAT4, drawTransform );
	graphicsShader->addUniformParameter( "color", ParameterType::PT_VEC4, drawColor );
	
	glm::vec2 uvOff = glm::vec2(0,0);
	glm::vec2 uvScale = glm::vec2(1,1);
	graphicsShader->setUniformParameter( "uvOff", ParameterType::PT_VEC2, &uvOff );
	graphicsShader->setUniformParameter( "uvScale", ParameterType::PT_VEC2, &uvScale );

	solidRect = solidRectGeo->makeRenderable(graphicsShader, white);
	emptyRect = emptyRectGeo->makeRenderable(graphicsShader, white);

	stencilConstrained = false;
}

Graphics::~Graphics()
{
	delete currentColor;
	delete currentTransform;
	delete drawColor;
	delete drawTransform;
}

void Graphics::setDepthTesting( bool depthTest )
{
	if(depthTest)
	{
		glEnable( GL_DEPTH_TEST );
		glDepthMask(true);
	}
	else
	{
		glDisable( GL_DEPTH_TEST );
		glDepthMask(false);
	}
}

void Graphics::drawRenderable( const Renderable* renderable )
{
	if (renderable->visible)
	{
		renderable->howShader->setUniformParameter( "world", ParameterType::PT_MAT4, currentTransform );

		glUseProgram( renderable->howShader->shaderProgramID );

		GLint loc = glGetUniformLocation( renderable->howShader->shaderProgramID, "useTexture");

		if( renderable->texture != NULL )
		{
			if (loc != -1)
				glUniform1i(loc, 1);

			glActiveTexture( GL_TEXTURE0 );

			glBindTexture( GL_TEXTURE_2D, renderable->texture->textureID );

			loc = glGetUniformLocation( renderable->howShader->shaderProgramID, "tex");
			glUniform1i(loc, 0);
		}
		else
		{
			if (loc != -1)
				glUniform1i(loc, 0);
		}

		loc = glGetUniformLocation( renderable->howShader->shaderProgramID, "useLighting");
		if( loc != -1 )
		{
			if( renderable->whatGeometry->indexingMode != GL_LINES )
			{
				glUniform1i(loc, 1);
			}
			else
			{
				glUniform1i(loc, 0);
			}
		}

		renderable->sendUniforms();
				
		glBindVertexArray( renderable->whatGeometry->vertexArrayID );
		glDrawElements( renderable->whatGeometry->indexingMode, renderable->whatGeometry->numIndices, GL_UNSIGNED_SHORT, (void*)renderable->whatGeometry->bufferOffset );
	}
}

void Graphics::clear(bool depthOnly)
{
	if( depthOnly )
		glClear( GL_DEPTH_BUFFER_BIT );
	else
		glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
}
void Graphics::setClearColor( float r, float g, float b, float a )
{
	glClearColor( r, g, b, a );
}

void Graphics::freeDrawSpace()
{
	if (stencilConstrained)
	{
		stencilConstrained = false;
		glStencilMask(0xFF);
		glClear( GL_STENCIL_BUFFER_BIT );
		glDisable(GL_STENCIL_TEST);
	}
}
void Graphics::constrainDrawSpace( float x, float y, float w, float h )
{
	if( !stencilConstrained )
	{
		glEnable(GL_STENCIL_TEST);
		stencilConstrained = true;
	}
	glStencilFunc(GL_NEVER, 1, 0xFF);
	glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);  // draw 1s on test fail (always)
 
	//Only affect stencil
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDepthMask(GL_FALSE);

	// draw stencil pattern
	fillRect( x, y, w, h );

	//Return control to color and depth
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glDepthMask(GL_TRUE);

	glStencilMask(0x00);
	// draw where stencil's value is 0
	//glStencilFunc(GL_EQUAL, 0, 0xFF);
	/* (nothing to draw) */
	// draw only where stencil's value is 1
	glStencilFunc(GL_EQUAL, 1, 0xFF);	
}

void Graphics::drawRect( const glm::vec2& position, const glm::vec2& dimensions )
{
	drawRect( position.x, position.y, dimensions.x, dimensions.y );
}
void Graphics::drawRect( float x, float y, float w, float h )
{
	*drawTransform = (*currentTransform) * glm::scale( glm::translate( glm::mat4(), glm::vec3(coordinateTransform->convert( glm::vec2( x,y ), CoordinateSystem::OPENGL_COORDINATES) - glm::vec2(0,coordinateTransform->scaleY(h,CoordinateSystem::OPENGL_COORDINATES)),0) ) ,glm::vec3(coordinateTransform->scale( glm::vec2( w,h ), CoordinateSystem::OPENGL_COORDINATES), 1 ) );
	//*currentColor = glm::vec4( r, g, b, a );

	drawRenderable( emptyRect );
}

void Graphics::fillRect( const glm::vec2& position, const glm::vec2& dimensions )
{
	fillRect( position.x, position.y, dimensions.x, dimensions.y );
}
void Graphics::fillRect( float x, float y, float w, float h )
{
	solidRect->texture = white;
	*drawTransform = (*currentTransform) * glm::scale( glm::translate( glm::mat4(), glm::vec3(coordinateTransform->convert( glm::vec2( x,y ), CoordinateSystem::OPENGL_COORDINATES) - glm::vec2(0,coordinateTransform->scaleY(h,CoordinateSystem::OPENGL_COORDINATES)),0) ) ,glm::vec3(coordinateTransform->scale( glm::vec2( w,h ), CoordinateSystem::OPENGL_COORDINATES), 1 ) );
	//*currentColor = glm::vec4( r, g, b, a );

	drawRenderable( solidRect );
}

void Graphics::drawImage( const glm::vec2& position, const glm::vec2& dimensions, const Texture* texture )
{
	drawImage( position.x, position.y, dimensions.x, dimensions.y, texture );
}
void Graphics::drawImage( float x, float y, float w, float h, const Texture* texture )
{
	solidRect->texture = texture;
	*drawTransform = (*currentTransform) * glm::scale( glm::translate( glm::mat4(), glm::vec3(coordinateTransform->convert( glm::vec2( x,y ), CoordinateSystem::OPENGL_COORDINATES) - glm::vec2(0,coordinateTransform->scaleY(h,CoordinateSystem::OPENGL_COORDINATES)),0) ) ,glm::vec3(coordinateTransform->scale( glm::vec2( w,h ), CoordinateSystem::OPENGL_COORDINATES), 1 ) );
	//*currentColor = glm::vec4( r, g, b, a );

	drawRenderable( solidRect );
}

void Graphics::drawText( const glm::vec2& position, const char * text, float screenSize )
{
	drawText( position.x, position.y, text, screenSize );
}

void Graphics::drawText( float x, float y, const char * text, float screenSize )
{
	const char *lastChar = text, *currentChar = text;

	float ratio = screenSize / currentFont->lineHeight;

	float currentX = x, currentY = y;

	while( *currentChar != '\0' )
	{
		Character* characterRepresent = currentFont->getCharacter( *currentChar );

		glm::vec2 uvOff = glm::vec2(characterRepresent->getX()/float(currentFont->width), 1 -( characterRepresent->height/float(currentFont->height) + characterRepresent->getY()/float(currentFont->height)));
		glm::vec2 uvScale = glm::vec2(characterRepresent->width/float(currentFont->width), characterRepresent->height/float(currentFont->height) ) * 1.0f;
		graphicsShader->setUniformParameter( "uvOff", ParameterType::PT_VEC2, &uvOff );
		graphicsShader->setUniformParameter( "uvScale", ParameterType::PT_VEC2,&uvScale );

		drawImage( currentX + (characterRepresent->xoff-128) * ratio, 
				currentY - (characterRepresent->yoff) * ratio - characterRepresent->height * ratio ,
				characterRepresent->width * ratio,
				characterRepresent->height * ratio, currentFont->texture );

		currentX += characterRepresent->advance * ratio;

		lastChar = currentChar;
		currentChar++;
	}

	glm::vec2 uvOff = glm::vec2(0,0);
	glm::vec2 uvScale = glm::vec2(1,1);
	graphicsShader->setUniformParameter( "uvOff", ParameterType::PT_VEC2, &uvOff );
	graphicsShader->setUniformParameter( "uvScale", ParameterType::PT_VEC2, &uvScale );
}

void Graphics::setColor( float r, float g, float b, float a )
{
	drawColor->r = r;
	drawColor->g = g;
	drawColor->b = b;
	drawColor->a = a;
}
void Graphics::setColor( const glm::vec4& color )
{
	*drawColor = color;
}
glm::vec4 Graphics::getColor() const
{
	return *currentColor;
}

void Graphics::setTransform( const glm::mat4& transform )
{
	*currentTransform = transform;
}

void Graphics::setFont( const Font* font )
{
	currentFont = font;
}
void Graphics::setFont()
{
	currentFont = defaultFont;
}

CoordinateTransform* Graphics::getCoordinateTransform() const
{
	return coordinateTransform;
}