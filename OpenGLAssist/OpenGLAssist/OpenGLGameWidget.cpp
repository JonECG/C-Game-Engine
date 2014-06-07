#include <GL\glew.h>

#include "OpenGLGameWidget.h"
#include <Qt\qtimer.h>

void OpenGLGameWidget::initializeGL()
{
	glewInit();
	init();

	timer.interval();

	QTimer qtimer;
	connect( &qtimer, SIGNAL(timeout()), this, SLOT(interval()) );
}

void OpenGLGameWidget::paintGL()
{
	paint();
}

void OpenGLGameWidget::interval()
{
	update( timer.interval() );
}