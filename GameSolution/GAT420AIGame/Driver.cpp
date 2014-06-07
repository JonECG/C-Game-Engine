#include <Qt\qapplication.h>


#include "AiGameWindow.h"
#include "DebugMenus.h"

int main( int argc, char* argv[] )
{
	QApplication app( argc, argv );

	AiGameWindow *baseWidg = new AiGameWindow();
	
	DebugMenus::menu = new DebugMenus();

	baseWidg->resize( 640, 640 );
	baseWidg->show();

	return app.exec();
}