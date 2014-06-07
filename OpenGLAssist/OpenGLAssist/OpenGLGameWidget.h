#include <QtOpenGL\qglwidget>
#include "Timer.h"

#pragma once
class OpenGLGameWidget : public QGLWidget
{
	Q_OBJECT

	Timer timer;

protected:
	void initializeGL();
	void paintGL();

	virtual void init() = 0;
	virtual void paint() = 0;
	virtual void update( float dt ) = 0;

private slots:
	void interval();
};