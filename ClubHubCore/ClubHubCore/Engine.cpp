#include "Engine.h"

#include "ManagedAppHandle.h"
#include <QtGui\qapplication.h>
#include "ManagedGLWidget.h"
#include <QtGui\qlayout.h>
#include "ContentManager.h"
#include "Graphics.h"

namespace Engine
{
	QApplication *app;
	ManagedAppHandle* handle;
	QWidget* base;
	void Engine::init( int argc, char* argv[] )
	{
		app = new QApplication( argc, argv );

		//QWidget *baseWidg = new QWidget();
		//QHBoxLayout *mainLayout = new QHBoxLayout();
		//baseWidg->setLayout( mainLayout );

		/*
		DebugMenus::menu = new DebugMenus();

		DebugMenus::inject( mainLayout );

		GameWidget *widg = new GameWidget();

		mainLayout->addWidget( widg, 100 );


		baseWidg->show();
		baseWidg->resize( 640, 640 );

		return app.exec();*/
	}

	void Engine::run( ManagedAppHandle* handle )
	{
		Engine::handle = handle;
		base = new QWidget();
		QHBoxLayout *mainLayout = new QHBoxLayout();
		base->setLayout( mainLayout );

		ManagedGLWidget *widg = new ManagedGLWidget();
		widg->setHandle( handle );

		mainLayout->addWidget( widg, 100 );


		base->show();
		base->resize( 640, 640 );

		app->exec();
	}

	void Engine::shutdown()
	{
		handle->shutdown();
		delete base;
		delete app;
	}
}
