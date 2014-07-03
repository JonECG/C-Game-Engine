#include <Qt\qapplication.h>


#include "GraphicsWindow.h"
#include "DebugMenus.h"

int main( int argc, char* argv[] )
{
	QApplication app( argc, argv );

	GraphicsWindow *baseWidg = new GraphicsWindow();
	
	DebugMenus::menu = new DebugMenus();

	

	baseWidg->resize( 640, 640 );
	baseWidg->show();

	DebugMenus::inject( baseWidg->layout() );
	DebugMenus::menu->show();

	return app.exec();
}