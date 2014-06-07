#pragma once

#include <QtOpenGL\qglwidget>

class OpenGLWindow : public QGLWidget
{
public:
	void paintGL();
	void initializeGL();
};

