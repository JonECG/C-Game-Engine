#include "GameWidget.h"
#include <Qt\qapplication.h>
#include <QtGui\qhboxlayout>
#include "DebugMenus.h"

int main( int argc, char* argv[] )
{
	QApplication app( argc, argv );

	QWidget *baseWidg = new QWidget();
	QHBoxLayout *mainLayout = new QHBoxLayout();
	baseWidg->setLayout( mainLayout );


	DebugMenus::menu = new DebugMenus();

	DebugMenus::inject( mainLayout );

	GameWidget *widg = new GameWidget();

	mainLayout->addWidget( widg, 100 );


	baseWidg->show();
	baseWidg->resize( 640, 640 );

	return app.exec();
}