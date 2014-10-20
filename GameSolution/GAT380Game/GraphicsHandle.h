#pragma once
#include "GeneralGLWindow.h"
#include "Qt/qmainwindow.h"

class GraphicsHandle : public GeneralGlWindow
{
	GeneralGlWindow::ShaderInfo *textShad;

protected:
	void init();
	void paint();
	void paintInner();
	void update( float dt );
};

