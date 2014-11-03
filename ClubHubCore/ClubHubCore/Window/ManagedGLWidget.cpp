#include <GL\glew.h>
#include "ManagedGLWidget.h"
#include "ManagedAppHandle.h"
#include "Graphics.h"
#include "ContentManager.h"
#include <Qt\qtimer.h>
#include "Timer.h"
#include "UserController.h"
#include <QtOpenGL\QGLWidget>
#include <QtGui\QMouseEvent>
#include <glm\glm.hpp>

#include "CoordinateTransform.h"
#include "ClubTime.h"

ManagedGLWidget::ManagedGLWidget()
{
	handle = nullptr;
	this->content = content;
	inited = false;
	timer = new QTimer();
	deltaTimer = new Timer();
}

ManagedGLWidget::~ManagedGLWidget()
{
	delete timer;
	delete deltaTimer;
	delete g;
	delete uc;
	delete handle;
	delete trans;
	delete content;
	//delete g;
}

void ManagedGLWidget::mousePressEvent( QMouseEvent * mEvent )
{
	mEvent;
	uc->setMouseDown( true );
}

void ManagedGLWidget::mouseMoveEvent( QMouseEvent * mEvent )
{
	uc->setMousePosition( glm::vec2( mEvent->x(), mEvent->y() ) );
}
void ManagedGLWidget::mouseReleaseEvent( QMouseEvent * mEvent )
{
	mEvent;
	uc->setMouseDown( false );
}

void ManagedGLWidget::setHandle( ManagedAppHandle *handle )
{
	if( inited )
		handle->init( content, trans, g );

	this->handle = handle;
}

void ManagedGLWidget::initializeGL()
{
	glewInit();

	glEnable( GL_TEXTURE_2D );
	glEnable( GL_DEPTH_TEST );

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.4f);

	//showFullScreen();
	//setWindowState( Qt::WindowFullScreen );// -> showFullScreen();

	content = new ContentManager();
	trans = new CoordinateTransform();
	g = new Graphics( content, trans, this );
	uc = new UserController( trans);

	setMouseTracking( true );

	inited = true;
	handle->init( content, trans, g );

	deltaTimer->interval();

	connect( timer, SIGNAL(timeout()), this, SLOT(interval()) );
	timer->start(0);
}

void ManagedGLWidget::paintGL()
{
	glViewport( 0, 0, width(), height() );
	trans->update( width(), height() );
	handle->paint(g);
	//DebugShapes::draw();
}

void ManagedGLWidget::interval()
{
	float dt = deltaTimer->interval();
	//DebugMenus::update();
	
	ClubTime::dt = dt;
	uc->update( dt );
	handle->update( dt, uc );
	
	repaint();
	//DebugShapes::update( dt );
}