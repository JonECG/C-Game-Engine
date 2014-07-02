#pragma once
#include "GeneralGLWindow.h"
#include "Qt/qmainwindow.h"

class GraphicsHandle : public GeneralGlWindow
{
	GeneralGlWindow::ShaderInfo *textShad;

	void mousePressEvent( QMouseEvent * mEvent );
	void mouseMoveEvent( QMouseEvent * mEvent );
	void mouseReleaseEvent( QMouseEvent * mEvent );

protected:
	void init();
	void paint();
	void update( float dt );
};

