#pragma once
#include "GeneralGLWindow.h"
class GameWidget : public GeneralGlWindow
{

	void mousePressEvent( QMouseEvent * mEvent );
	void mouseMoveEvent( QMouseEvent * mEvent );
	void mouseReleaseEvent( QMouseEvent * mEvent );

protected:
	void init();
	void paint();
	void update( float dt );
};

