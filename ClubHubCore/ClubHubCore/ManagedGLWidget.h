#pragma once

#include <QtOpenGL\QGLWidget>

class Graphics;
class UserController;
class ContentManager;
class ManagedAppHandle;
class QTimer;
class Timer;
class CoordinateTransform;

class ManagedGLWidget : public QGLWidget
{
	Q_OBJECT

	ManagedAppHandle *handle;
	ContentManager *content;
	Graphics *g;
	UserController *uc;

	QTimer *timer;
	Timer *deltaTimer;

	bool inited;

	CoordinateTransform *trans;

	void mousePressEvent( QMouseEvent * mEvent );
	void mouseMoveEvent( QMouseEvent * mEvent );
	void mouseReleaseEvent( QMouseEvent * mEvent );

public:
	ManagedGLWidget();
	~ManagedGLWidget();
	void initializeGL();
	void paintGL();
	void setHandle( ManagedAppHandle *handle );

private slots:
	void interval();
};

