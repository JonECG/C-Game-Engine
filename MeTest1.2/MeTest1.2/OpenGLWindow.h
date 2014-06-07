#pragma once
#include <QtOpenGL\qglwidget>

class OpenGLWindow : public QGLWidget
{
	Q_OBJECT

public:
	void paintGL();
	void initializeGL();

private slots:
	void updateWindow();
};

