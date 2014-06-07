#pragma once
#include "GeneralGLWindow.h"
#include "Qt/qmainwindow.h"
#include "WayMapLevel.h"
#include "AStarDebugger.h"
#include "GameWorld.h"

class AiGameHandle : public GeneralGlWindow
{
	GameWorld * env;
	GeneralGlWindow::ShaderInfo *textShad;

	AStarDebugger * debugger;

	void mousePressEvent( QMouseEvent * mEvent );
	void mouseMoveEvent( QMouseEvent * mEvent );
	void mouseReleaseEvent( QMouseEvent * mEvent );

protected:
	void init();
	void paint();
	void update( float dt );
};

