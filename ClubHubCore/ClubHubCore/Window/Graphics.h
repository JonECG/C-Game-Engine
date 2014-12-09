#pragma once

#include "definition.h"
#include "glmfwd.h"

#include <Windows.h>

struct Renderable;
struct Texture;
struct Font;
struct FrameBufferObject;
struct Shader;
class ContentManager;
class QWidget;
class ManagedGLWidget;
class CoordinateTransform;


class EXPORT Graphics
{
	bool stencilConstrained;

	Renderable *solidRect, *emptyRect;

	glm::vec4 *currentColor, *drawColor;
	glm::mat4 *currentTransform, *drawTransform;

	Texture *white;
	const Font *defaultFont, *currentFont;
	ManagedGLWidget * handle;
	QWidget * window;

	Shader *graphicsShader;

	CoordinateTransform *coordinateTransform;
public:
	Graphics( ContentManager* content, CoordinateTransform *transform, ManagedGLWidget * handle );
	~Graphics();
	void drawRenderable( const Renderable* renderable );

	void clear(bool depthOnly = false);
	void setClearColor( float r, float g, float b, float a );

	void setDepthTesting( bool depthTest );

	int getWidth();
	int getHeight();

	void freeDrawSpace();
	void constrainDrawSpace( float x, float y, float w, float h );

	void setViewport( int x, int y, int w, int h );
	void setFrameBuffer( FrameBufferObject * fbo = 0 );

	void drawRect( float x, float y, float w, float h );
	void drawRect( const glm::vec2& position, const glm::vec2& dimensions );
	//void drawRect( float x, float y, float w, float h, float r = 1, float g = 1, float b = 1, float a = 1 );
	//void drawRect( float x, float y, float w, float h, const glm::mat4& transform, float r = 1, float g = 1, float b = 1, float a = 1 );
	void fillRect( float x, float y, float w, float h );
	void fillRect( const glm::vec2& position, const glm::vec2& dimensions );
	//void fillRect( float x, float y, float w, float h, float r = 1, float g = 1, float b = 1, float a = 1 );
	//void fillRect( float x, float y, float w, float h, const glm::mat4 transform, float r = 1, float g = 1, float b = 1, float a = 1 );

	void drawImage( float x, float y, float w, float h, const Texture* texture );
	void drawImage( const glm::vec2& position, const glm::vec2& dimensions, const Texture* texture );
	//void drawImage( float x, float y, float w, float h, const Texture* texture, const glm::mat4& transform );

	void drawText( const glm::vec2& position, const char * text, float screenSize = 0.1 );
	void drawText( float x, float y, const char * text, float screenSize = 0.1 );
	//void drawText( float x, float y, const char * text, const glm::mat4& transform );

	glm::vec4 getColor() const;
	CoordinateTransform *getCoordinateTransform() const;
	void setColor( float r, float g, float b, float a );
	void setColor( const glm::vec4& color );

	void setTransform( const glm::mat4& transform );

	void setFont( const Font* font );
	void setFont();

	HWND getHandle();
	HDC getHDC();

	void showFullscreen();

	void setAutoBufferSwap( bool autoSwap );
};

