#pragma once
#include "GeneralGLWindow.h"
#include "Qt/qmainwindow.h"
class LevelEditorHandle : public GeneralGlWindow
{
	GeneralGlWindow::Renderable * levelRenderable;
	glm::vec3 * positions;
	int currentRenderable;
	GeneralGlWindow::ShaderInfo *textShad, *colorShad;
	GeneralGlWindow::TextureInfo* levelTexture;

	GeneralGlWindow::GeometryInfo * sphereGeometry;
	GeneralGlWindow::TextureInfo * sphereTexture;
	GeneralGlWindow::Renderable ** sphereRenderables;

	float SPHERE_LENGTH;

	int currentConnection;
	int connections[1000];

	friend class LevelEditorWindow;

	void mousePressEvent( QMouseEvent * mEvent );
	void mouseMoveEvent( QMouseEvent * mEvent );
	void mouseReleaseEvent( QMouseEvent * mEvent );

	bool connectionExists( int from, int to );

protected:
	void init();
	void paint();
	void update( float dt );
};

