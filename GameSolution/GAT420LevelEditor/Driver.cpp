#include <Qt\qapplication.h>


#include "LevelEditorWindow.h"
#include "DebugMenus.h"

#include "GL\glew.h"

int main( int argc, char* argv[] )
{
	QApplication app( argc, argv );

	LevelEditorWindow *baseWidg = new LevelEditorWindow();
	
	DebugMenus::menu = new DebugMenus();

	baseWidg->resize( 640, 640 );
	baseWidg->show();

	return app.exec();
}